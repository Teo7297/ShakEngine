#include "ShakEngine.h"
#include "GameObject.h"

namespace shak
{
    GameObject::GameObject()
        : m_vertices(nullptr), m_texture(nullptr), m_shader(nullptr), m_parent(nullptr), m_active(true), m_followParent(true), m_zIndex(100)
    {
        m_engine = &ShakEngine::GetInstance();
        Id = m_engine->GetNextGameObjectId();
    }

    GameObject::GameObject(std::shared_ptr<sf::VertexArray> va, std::shared_ptr<sf::Texture> texture)
        : m_vertices(va), m_texture(texture), m_shader(nullptr), m_parent(nullptr), m_active(true), m_followParent(true), m_zIndex(100)
    {
        m_engine = &ShakEngine::GetInstance();
        Id = m_engine->GetNextGameObjectId();
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
    }

    void GameObject::move(sf::Vector2f offset)
    {
        Transformable::move(offset);
        for (const auto& [id, child] : m_children)
        {
            if (child->GetFollowParent())
                child->move(offset);
        }
        m_movedThisFrame = true;
    }

    void GameObject::rotate(sf::Angle angle)
    {
        Transformable::rotate(angle);
        for (const auto& [id, child] : m_children)
        {
            if (!child->GetFollowParent()) continue;

            sf::Vector2f relativePos = child->getPosition() - this->getPosition();

            // Calculate the new position after rotation (remember, we are using global coordinates, with global axis for translation!)
            float radians = angle.asRadians();
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
        for (const auto& [id, child] : m_children)
        {
            child->scale(factor);
        }
    }

    void GameObject::setOrigin(const sf::Vector2f& origin)
    {
        Transformable::setOrigin(origin);
        this->move(origin);
    }

    void GameObject::AddChild(GameObjectPtr child)
    {
        if (m_children.find(child->Id) != m_children.end())
            return;
        m_children[child->Id] = child;
        child->SetParent(this);
        if (child->m_physicsEnabled)
            m_engine->GetScene()->AddGameObjectToQuadtree(child);
    }

    bool GameObject::RemoveChild(int id)
    {
        if (m_children.find(id) == m_children.end())
            return false;

        m_children.erase(id);
        return true;
    }

    bool GameObject::RemoveChildRecursive(int id)
    {
        if (RemoveChild(id))
            return true;
        else
            for (const auto& [id, child] : m_children)
                if (child->RemoveChildRecursive(id))
                    return true;

        return false;
    }

    GameObjectPtr GameObject::FindChildRecursive(std::string name) const
    {
        for (const auto& [id, child] : m_children)
        {
            if (child->Name == name)
                return child;
            else
            {
                auto found = child->FindChildRecursive(name);
                if (found)
                    return found;
            }
        }
        return nullptr;
    }

    GameObjectPtr GameObject::FindChildRecursive(int id) const
    {
        if (m_children.find(id) != m_children.end())
            return m_children.at(id);

        for (const auto& [id, child] : m_children)
        {
            auto found = child->FindChildRecursive(id);
            if (found)
                return found;
        }

        return nullptr;
    }

    void GameObject::GetDrawables(std::vector<GameObjectPtr>& drawables) const
    {
        for (const auto& [id, child] : m_children)
        {
            if (child->IsActive())
            {
                drawables.emplace_back(child);
                child->GetDrawables(drawables);
            }
        }
    }

    std::vector<GameObjectPtr> GameObject::GetChildren() const
    {
        std::vector<GameObjectPtr> result;
        result.reserve(m_children.size());
        for (const auto& [id, child] : m_children)
            result.emplace_back(child);
        return result;
    }

    void GameObject::SetColor(const sf::Color& color)
    {
        for (int i = 0; i < m_vertices->getVertexCount(); i++)
        {
            auto& vertex = (*m_vertices)[i];
            vertex.color = color;
        }
    }

    void GameObject::SetTransparency(uint8_t transparency)
    {
        for (int i = 0; i < m_vertices->getVertexCount(); i++)
        {
            auto& vertex = (*m_vertices)[i];
            vertex.color.a = transparency;
        }
    }

    void GameObject::Awake() // TODO: Awake should be also called when an object gets created at runtime
    {
        if (!m_safeChildrenCopied)
        {
            m_safeChildren = GetChildren();
            m_safeChildrenCopied = true;
        }
        for (const auto& child : m_safeChildren)
        {
            if (child->IsActive())
                child->Awake();
        }
    }

    void GameObject::Update(float dt)
    {
        if (!m_safeChildrenCopied)
        {
            m_safeChildren = GetChildren();
            m_safeChildrenCopied = true;
        }
        for (const auto& child : m_safeChildren)
        {
            if (child->IsActive())
                child->Update(dt);
        }
    }

    void GameObject::Cleanup()
    {
        m_safeChildrenCopied = false;
        for (const auto& [id, child] : m_children)
        {
            child->Cleanup();
        }
    }

    void GameObject::HandleInput(const sf::Event& event)
    {
        if (!m_safeChildrenCopied)
        {
            m_safeChildren = GetChildren();
            m_safeChildrenCopied = true;
        }
        for (const auto& child : m_safeChildren)
        {
            if (child->IsActive())
                child->HandleInput(event);
        }
    }

    void GameObject::OnDestroy()
    {
        if (!m_safeChildrenCopied)
        {
            m_safeChildren = GetChildren();
            m_safeChildrenCopied = true;
        }
        for (const auto& child : m_safeChildren)
        {
            if (child->IsActive())
                child->OnDestroy();
        }
    }

    bool GameObject::IsPointInside(const sf::Vector2f& worldPoint) const
    {
        const auto bounds = m_vertices->getBounds();
        const auto center = bounds.size * 0.5f; // size / 2
        const auto zeroPosition = worldPoint - this->getPosition();

        return m_vertices->getBounds().contains(zeroPosition + center);
    }

    bool GameObject::IsCollidingWithGameObject(const GameObjectPtr& other) const
    {
        const auto zeroPosition = other->getPosition() - this->getPosition();
        return m_vertices->getBounds().findIntersection({ zeroPosition, other->GetVertexArray()->getBounds().size }).has_value();
    }

    bool GameObject::IsCollidingWitRect(const sf::FloatRect& rect) const
    {
        const auto zeroPosition = rect.position - this->getPosition();
        return m_vertices->getBounds().findIntersection({ zeroPosition, rect.size }).has_value();
    }
}