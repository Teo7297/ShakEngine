#include "GameObject.h"

namespace shak
{
    GameObject::GameObject(std::shared_ptr<sf::VertexArray> va, std::shared_ptr<sf::Texture> texture)
        : m_vertices(va), m_texture(texture)
    {
    }

    void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= this->getTransform();
        if (m_texture)
            states.texture = m_texture.get();
        if (m_shader)
            states.shader = m_shader.get();
        if (m_vertices)
            target.draw(*m_vertices, states);

        for (const auto& child : m_children)
        {
            if (child->IsActive())
                child->draw(target, sf::RenderStates::Default);
        }
    }

    void GameObject::move(sf::Vector2f offset)
    {
        Transformable::move(offset);
        for (const auto& child : m_children)
        {
            if (child->GetFollowParent())
                child->move(offset);
        }
    }

    void GameObject::rotate(sf::Angle angle)
    {
        Transformable::rotate(angle);
        for (const auto& child : m_children)
        {
            if (!child->GetFollowParent()) continue;

            sf::Vector2f relativePos = child->getPosition() - this->getPosition();

            // Calculate the new position after rotation (remember, we are using global coordinates, with global axis for translation!)
            float radians = angle.asDegrees() * (3.14159265f / 180.f);
            float cosAngle = std::cos(radians);
            float sinAngle = std::sin(radians);
            sf::Vector2f rotatedPos(
                relativePos.x * cosAngle - relativePos.y * sinAngle,
                relativePos.x * sinAngle + relativePos.y * cosAngle
            );

            child->setPosition(this->getPosition() + rotatedPos);
            child->rotate(angle);
        }

    }

    void GameObject::rotateAround(sf::Angle angle, const sf::Vector2f& pivot)
    {
        sf::Vector2f relativePos = this->getPosition() - pivot;

        // Calculate the new position after rotation (remember, we are using global coordinates, with global axis for translation!)
        float radians = angle.asDegrees() * (3.14159265f / 180.f);
        float cosAngle = std::cos(radians);
        float sinAngle = std::sin(radians);
        sf::Vector2f rotatedPos(
            relativePos.x * cosAngle - relativePos.y * sinAngle,
            relativePos.x * sinAngle + relativePos.y * cosAngle
        );

        this->setPosition(pivot + rotatedPos);
        this->rotate(angle);
    }

    void GameObject::scale(sf::Vector2f factor)
    {
        Transformable::scale(factor);
        for (const auto& child : m_children)
        {
            child->scale(factor);
        }
    }

    void GameObject::setOrigin(const sf::Vector2f& origin)
    {
        Transformable::setOrigin(origin);
        this->move(origin);
    }

    void GameObject::AddChild(std::shared_ptr<GameObject> child)
    {
        for (auto& c : m_children)
        {
            if (c == child)
                return;
        }
        m_children.push_back(child);
    }

    void GameObject::RemoveChild(int id)
    {
        for (auto it = m_children.begin(); it != m_children.end(); ++it)
        {
            if ((*it)->Id == id)
            {
                m_children.erase(it);
                return;
            }
        }
    }

    std::vector<std::shared_ptr<GameObject>> GameObject::GetChildren() const
    {
        return std::vector<std::shared_ptr<GameObject>>(m_children);
    }

    void GameObject::Awake() // TODO: Awake should be also called when an object gets created at runtime
    {
        for (const auto& child : m_children)
        {
            if (child->IsActive())
                child->Awake();
        }
    }

    void GameObject::Update(float dt)
    {
        for (const auto& child : m_children)
        {
            if (child->IsActive())
                child->Update(dt);
        }
    }

    void GameObject::HandleInput(const sf::Event& event)
    {
        for (const auto& child : m_children)
        {
            child->HandleInput(event);
        }
    }

    bool GameObject::IsPointInside(const sf::Vector2f& worldPoint) const
    {
        const auto bounds = m_vertices->getBounds();
        const auto center = bounds.size * 0.5f; // size / 2
        const auto zeroPosition = worldPoint - this->getPosition();

        return m_vertices->getBounds().contains(zeroPosition + center);
    }
}