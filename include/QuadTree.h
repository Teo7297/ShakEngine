#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    class Quadtree
    {
    public:
        Quadtree(const sf::FloatRect& box) :
            mBox(box), mRoot(std::make_unique<Node>())
        {

        }

        void add(const GameObjectPtr& value)
        {
            // std::cout << "Adding " << value->Name << " to quadtree" << std::endl;
            add(mRoot.get(), 0, mBox, value);
        }

        void remove(const GameObjectPtr& value)
        {
            remove(mRoot.get(), mBox, value);
        }

        void clear()
        {
            clearNode(mRoot.get());
            mRoot = std::make_unique<Node>();
        }

        void update()
        {
            updateAll(mRoot.get());
        }

        std::vector<GameObjectPtr> query(const sf::FloatRect& box) const
        {
            auto values = std::vector<GameObjectPtr>();
            query(mRoot.get(), mBox, box, values);
            return values;
        }

        std::vector<std::pair<GameObjectPtr, GameObjectPtr>> findAllIntersections() const
        {
            auto intersections = std::vector<std::pair<GameObjectPtr, GameObjectPtr>>();
            findAllIntersections(mRoot.get(), intersections);
            return intersections;
        }

        sf::FloatRect getBox() const
        {
            return mBox;
        }

    private:
        // We shouldn't be able to hit these values anyways, just let the tree grow if needed
        static constexpr auto Threshold = std::size_t(1600);
        static constexpr auto MaxDepth = std::size_t(800);

        struct Node
        {
            ~Node() { std::cout << "Node destroyed" << std::endl; }
            std::array<std::unique_ptr<Node>, 4> children;
            std::vector<GameObjectPtr> values;
        };

        sf::FloatRect mBox;
        std::unique_ptr<Node> mRoot;

        bool isLeaf(const Node* node) const
        {
            return !static_cast<bool>(node->children[0]);
        }

        sf::FloatRect computeBox(const sf::FloatRect& box, int i) const
        {
            auto origin = box.position;
            auto childSize = box.size / static_cast<float>(2);
            switch (i)
            {
                // North West
            case 0:
                return sf::FloatRect(origin, childSize);
                // Norst East
            case 1:
                return sf::FloatRect({ origin.x + childSize.x, origin.y }, childSize);
                // South West
            case 2:
                return sf::FloatRect({ origin.x, origin.y + childSize.y }, childSize);
                // South East
            case 3:
                return sf::FloatRect(origin + childSize, childSize);
            default:
                assert(false && "Invalid child index");
                return sf::FloatRect();
            }
        }

        int getQuadrant(const sf::FloatRect& nodeBox, const sf::FloatRect& valueBox) const
        {
            auto center = nodeBox.getCenter();
            // West
            if (valueBox.position.x + valueBox.size.x < center.x)
            {
                // North West
                if (valueBox.position.y + valueBox.size.y < center.y)
                    return 0;
                // South West
                else if (valueBox.position.y >= center.y)
                    return 2;
                // Not contained in any quadrant
                else
                    return -1;
            }
            // East
            else if (valueBox.position.x >= center.x)
            {
                // North East
                if (valueBox.position.y + valueBox.size.y < center.y)
                    return 1;
                // South East
                else if (valueBox.position.y >= center.y)
                    return 3;
                // Not contained in any quadrant
                else
                    return -1;
            }
            // Not contained in any quadrant
            else
                return -1;
        }

        void add(Node* node, std::size_t depth, const sf::FloatRect& box, const GameObjectPtr& value)
        {
            if (isLeaf(node))
            {
                // Insert the value in this node if possible
                if (depth >= MaxDepth || node->values.size() < Threshold)
                    node->values.push_back(value);
                // Otherwise, we split and we try again
                else
                {
                    split(node, box);
                    add(node, depth, box, value);
                }
            }
            else
            {
                auto i = getQuadrant(box, value->GetVertexArray()->getBounds());
                // Add the value in a child if the value is entirely contained in it
                if (i != -1)
                    add(node->children[static_cast<std::size_t>(i)].get(), depth + 1, computeBox(box, i), value);
                // Otherwise, we add the value in the current node
                else
                    node->values.push_back(value);
            }
        }

        void split(Node* node, const sf::FloatRect& box)
        {
            assert(node != nullptr);
            assert(isLeaf(node) && "Only leaves can be split");
            // Create children
            for (auto& child : node->children)
                child = std::make_unique<Node>();
            // Assign values to children
            auto newValues = std::vector<GameObjectPtr>(); // New values for this node
            for (const auto& value : node->values)
            {
                auto i = getQuadrant(box, value->GetVertexArray()->getBounds());
                if (i != -1)
                    node->children[static_cast<std::size_t>(i)]->values.push_back(value);
                else
                    newValues.push_back(value);
            }
            node->values = std::move(newValues);
        }

        bool remove(Node* node, const sf::FloatRect& box, const GameObjectPtr& value)
        {
            if (isLeaf(node))
            {
                // Remove the value from node
                removeValue(node, value);
                return true;
            }
            else
            {
                // Remove the value in a child if the value is entirely contained in it
                auto i = getQuadrant(box, value->GetVertexArray()->getBounds());
                if (i != -1)
                {
                    if (remove(node->children[static_cast<std::size_t>(i)].get(), computeBox(box, i), value))
                        return tryMerge(node);
                }
                // Otherwise, we remove the value from the current node
                else
                    removeValue(node, value);
                return false;
            }
        }

        void removeValue(Node* node, const GameObjectPtr& value)
        {
            // Find the value in node->values
            auto it = std::find_if(std::begin(node->values), std::end(node->values),
                [this, &value](const auto& rhs) { return value->Id == rhs->Id; });
            assert(it != std::end(node->values) && "Trying to remove a value that is not present in the node");
            // Swap with the last element and pop back
            *it = std::move(node->values.back());
            node->values.pop_back();
        }

        bool tryMerge(Node* node)
        {
            assert(node != nullptr);
            assert(!isLeaf(node) && "Only interior nodes can be merged");
            auto nbValues = node->values.size();
            for (const auto& child : node->children)
            {
                if (!isLeaf(child.get()))
                    return false;
                nbValues += child->values.size();
            }
            if (nbValues <= Threshold)
            {
                node->values.reserve(nbValues);
                // Merge the values of all the children
                for (const auto& child : node->children)
                {
                    for (const auto& value : child->values)
                        node->values.push_back(value);
                }
                // Remove the children
                for (auto& child : node->children)
                    child.reset();
                return true;
            }
            else
                return false;
        }

        void updateAll(Node* node)
        {
            for (auto i = std::size_t(0); i < node->values.size(); ++i)
            {
                auto obj = node->values[i];
                if (!obj->HasMovedThisFrame()) continue;

                obj->ResetMovedThisFrame();
                remove(obj);
                add(obj);
            }
            if (!isLeaf(node))
            {
                for (const auto& child : node->children)
                    updateAll(child.get());
            }
        }

        void clearNode(Node* node)
        {
            if (!node)
                return;

            // First, clear all children recursively
            for (auto& child : node->children)
            {
                if (child)
                {
                    clearNode(child.get());
                    child.reset();
                }
            }
            // Then clear the values at this node
            node->values.clear();
        }

        void query(Node* node, const sf::FloatRect& box, const sf::FloatRect& queryBox, std::vector<GameObjectPtr>& values) const
        {
            for (const auto& value : node->values)
            {
                if (value->IsCollidingWitRect(queryBox))
                    values.push_back(value);
            }
            if (!isLeaf(node))
            {
                for (auto i = std::size_t(0); i < node->children.size(); ++i)
                {
                    auto childBox = computeBox(box, static_cast<int>(i));
                    if (childBox.findIntersection(queryBox).has_value())
                        query(node->children[i].get(), childBox, queryBox, values);
                }
            }
        }

        void findAllIntersections(Node* node, std::vector<std::pair<GameObjectPtr, GameObjectPtr>>& intersections) const
        {
            // Find intersections between values stored in this node
            // Make sure to not report the same intersection twice
            for (auto i = std::size_t(0); i < node->values.size(); ++i)
            {
                for (auto j = std::size_t(0); j < i; ++j)
                {
                    if (node->values[i]->IsCollidingWithGameObject(node->values[j]))
                        intersections.emplace_back(node->values[i], node->values[j]);
                }
            }
            if (!isLeaf(node))
            {
                // Values in this node can intersect values in descendants
                for (const auto& child : node->children)
                {
                    for (const auto& value : node->values)
                        findIntersectionsInDescendants(child.get(), value, intersections);
                }
                // Find intersections in children
                for (const auto& child : node->children)
                    findAllIntersections(child.get(), intersections);
            }
        }

        void findIntersectionsInDescendants(Node* node, const GameObjectPtr& value, std::vector<std::pair<GameObjectPtr, GameObjectPtr>>& intersections) const
        {
            // Test against the values stored in this node
            for (const auto& other : node->values)
            {
                if (value->IsCollidingWithGameObject(other))
                    intersections.emplace_back(value, other);
            }
            // Test against values stored into descendants of this node
            if (!isLeaf(node))
            {
                for (const auto& child : node->children)
                    findIntersectionsInDescendants(child.get(), value, intersections);
            }
        }
    };

}