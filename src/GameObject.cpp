#include "ShakEngine.h"
#include "GameObject.h"

namespace shak
{
    GameObject::GameObject()
        : GameObject(nullptr, nullptr)
    {
    }

    GameObject::GameObject(std::shared_ptr<sf::VertexArray> va, std::shared_ptr<sf::Texture> texture)
        : m_vertices(va)
        , m_texture(texture)
        , m_shader(nullptr)
        , m_parent(nullptr)
        , m_safeCopyDone(false)
        , m_children()
        , m_components()
        , m_safeChildren()
        , m_safeComponents()
        , m_active(true)
        , m_needAwake(true)
        , m_followParent(true)
        , m_rotateWithParent(true)
        , m_zIndex(100)
        , m_physicsEnabled(false)
        , m_movedThisFrame(false)
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

    void GameObject::setPosition(sf::Vector2f position)
    {
        Transformable::setPosition(position);
        for (const auto& [id, child] : m_children)
        {
            if (child->GetFollowParent())
                child->setPosition(position);
        }
        m_movedThisFrame = true;
    }

    void GameObject::rotate(sf::Angle angle)
    {
        Transformable::rotate(angle);
        for (const auto& [id, child] : m_children)
        {
            if (!child->GetRotateWithParent()) continue;

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

    void GameObject::setRotation(sf::Angle angle)
    {
        Transformable::setRotation(angle);
        for (const auto& [id, child] : m_children)
        {
            if (!child->GetRotateWithParent()) continue;

            sf::Vector2f relativePos = child->getPosition() - this->getPosition();

            // Calculate the new position after rotation (remember, we are using global coordinates, with global axis for translation!)
            auto actualAngle = angle - child->getRotation();
            float radians = actualAngle.asRadians();
            float cosAngle = std::cos(radians);
            float sinAngle = std::sin(radians);
            sf::Vector2f rotatedPos(
                relativePos.x * cosAngle - relativePos.y * sinAngle,
                relativePos.x * sinAngle + relativePos.y * cosAngle
            );

            child->setPosition(this->getPosition() + rotatedPos);
            child->setRotation(angle);
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
    }

    void GameObject::CenterOrigin()
    {
        this->setOrigin(m_vertices->getBounds().getCenter());
    }

    void GameObject::AddChild(const GameObjectPtr& child)
    {
        if (m_children.contains(child->Id))
            return;
        m_children[child->Id] = child;
        child->SetParent(this);
        if (child->m_physicsEnabled)
            m_engine->GetScene()->AddGameObjectToQuadtree(child);
    }

    bool GameObject::RemoveChild(int id)
    {
        if (!m_children.contains(id))
            return false;

        m_children[id]->InternalDestroy();
        m_children[id]->RemoveChildrenRecursive(); // delete all hierarchy below
        m_children.erase(id);
        return true;
    }

    bool GameObject::RemoveChildRecursive(int id)
    {
        if (RemoveChild(id))
            return true;
        else
            for (const auto& [_, child] : m_children)
                if (child->RemoveChildRecursive(id))
                    return true;

        return false;
    }

    void GameObject::RemoveChildrenRecursive()
    {
        for (const auto& [_, child] : m_children)
        {
            child->RemoveChildrenRecursive();
            child->RemoveChildren();
        }
    }

    void GameObject::RemoveChildren()
    {
        m_children.clear();
    }

    GameObjectPtr GameObject::FindChildRecursive(const std::string& name) const
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
        if (m_children.contains(id))
            return m_children.at(id);

        for (const auto& [_, child] : m_children)
        {
            if (auto found = child->FindChildRecursive(id))
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

    void GameObject::SetActive(bool active)
    {
        m_active = active;
        if (m_active)
            OnEnable();
        else
            OnDisable();
    }

    std::vector<GameObjectPtr> GameObject::GetChildren() const
    {
        std::vector<GameObjectPtr> result;
        result.reserve(m_children.size());
        for (const auto& [id, child] : m_children)
            result.emplace_back(child);
        return result;
    }

    std::vector<std::shared_ptr<Component>> GameObject::GetComponents() const
    {
        std::vector<std::shared_ptr<Component>> result;
        result.reserve(m_components.size());
        for (const auto& [id, comp] : m_components)
            result.emplace_back(comp);
        return result;
    }

    void GameObject::InitQuadVertexArray(sf::Vector2f size, sf::Color color)
    {
        m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
        (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
        (*m_vertices)[1].position = sf::Vector2f(0.f, 250.f);
        (*m_vertices)[2].position = sf::Vector2f(250.f, 0.f);
        (*m_vertices)[3].position = sf::Vector2f(250.f, 250.f);
        (*m_vertices)[0].color = sf::Color::White;
        (*m_vertices)[1].color = sf::Color::White;
        (*m_vertices)[2].color = sf::Color::White;
        (*m_vertices)[3].color = sf::Color::White;
    }

    void GameObject::SetColor(const sf::Color& color) const
    {
        for (size_t i = 0; i < m_vertices->getVertexCount(); i++)
        {
            auto& vertex = (*m_vertices)[i];
            vertex.color = color;
        }
    }

    void GameObject::SetTransparency(uint8_t transparency) const
    {
        for (size_t i = 0; i < m_vertices->getVertexCount(); i++)
        {
            auto& vertex = (*m_vertices)[i];
            vertex.color.a = transparency;
        }
    }

    void GameObject::ForwardAwake()
    {
        TrySafeCopy();
        for (const auto& comp : m_safeComponents)
        {
            if (comp->IsActive() && comp->NeedAwake())
            {
                comp->Awake();
                comp->shak::Component::Awake();
            }
        }

        for (const auto& child : m_safeChildren)
        {
            if (child->IsActive())
            {
                if (child->NeedAwake())
                {
                    child->Awake();
                    child->shak::GameObject::Awake();
                }

                child->ForwardAwake();
            }
        }
    }

    void GameObject::Awake()
    {
        m_needAwake = false;
    }

    void GameObject::InternalUpdate(float dt)
    {
        TrySafeCopy();
        for (const auto& comp : m_safeComponents)
        {
            if (comp->IsActive())
            {
                comp->Update(dt);
                comp->shak::Component::Update(dt);
            }
        }

        for (const auto& child : m_safeChildren)
        {
            if (child->IsActive())
            {
                child->Update(dt);
                child->InternalUpdate(dt);
            }
        }

        // Set basic shader uniforms
        if (m_shader)
        {
            m_shader->setUniform("u_time", m_engine->GetTime());
            m_shader->setUniform("u_resolution", sf::Glsl::Vec2{ m_engine->GetWindowSize().x, m_engine->GetWindowSize().y });
        }
    }

    void GameObject::InternalLateUpdate(float dt)
    {
        TrySafeCopy();
        for (const auto& comp : m_safeComponents)
        {
            if (comp->IsActive())
            {
                comp->LateUpdate(dt);
                comp->shak::Component::LateUpdate(dt);
            }
        }

        for (const auto& child : m_safeChildren)
        {
            if (child->IsActive())
            {
                child->LateUpdate(dt);
                child->InternalLateUpdate(dt);
            }
        }
    }

    void GameObject::InternalCleanup()
    {
        m_safeCopyDone = false;
        for (const auto& [id, child] : m_children)
        {
            child->Cleanup();
            child->InternalCleanup();
        }
    }

    void GameObject::InternalHandleInput(const sf::Event& event)
    {
        TrySafeCopy();
        for (const auto& comp : m_safeComponents)
        {
            if (comp->IsActive())
            {
                comp->HandleInput(event);
                comp->shak::Component::HandleInput(event);
            }
        }

        for (const auto& child : m_safeChildren)
        {
            if (child->IsActive())
            {
                child->HandleInput(event);
                child->InternalHandleInput(event);
            }
        }
    }

    void GameObject::InternalDestroy()
    {
        TrySafeCopy();
        for (const auto& comp : m_safeComponents)
        {
            if (comp->IsActive())
            {
                comp->OnDestroy();
                comp->shak::Component::OnDestroy();
            }
        }

        for (const auto& child : m_safeChildren)
        {
            if (child->IsActive())
            {
                child->OnDestroy();
                child->InternalDestroy();
            }
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
        const auto zeroPosition = (other->getPosition() - other->getOrigin()) - (this->getPosition() - this->getOrigin());
        return m_vertices->getBounds().findIntersection({ zeroPosition, other->GetVertexArray()->getBounds().size }).has_value();
    }

    bool GameObject::IsCollidingWitRect(const sf::FloatRect& rect) const
    {
        const auto zeroPosition = rect.position - (this->getPosition() - this->getOrigin());
        return m_vertices->getBounds().findIntersection({ zeroPosition, rect.size }).has_value();
    }

    void GameObject::TrySafeCopy()
    {
        if (m_safeCopyDone) return;

        m_safeChildren = GetChildren();
        m_safeComponents = GetComponents();
        m_safeCopyDone = true;
    }
}