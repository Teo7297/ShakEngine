#include "GameObject.h"

namespace shak
{
    GameObject::GameObject(std::shared_ptr<sf::VertexArray> va, std::shared_ptr<sf::Texture> texture)
        : m_vertices(va), m_texture(texture)
    {
    }

    void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        states.transform *= this->getTransform();
        if (m_texture)
            states.texture = m_texture.get();
        if (m_shader)
            states.shader = m_shader.get();
        target.draw(*m_vertices, states);

        for (const auto& child : m_children)
        {
            child->draw(target, sf::RenderStates::Default);
        }
    }

    void GameObject::move(sf::Vector2f offset)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        Transformable::move(offset);
        for (const auto& child : m_children)
        {
            child->move(offset);
        }
    }

    void GameObject::rotate(sf::Angle angle)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        Transformable::rotate(angle);
        for (const auto& child : m_children)
        {
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

    void GameObject::scale(sf::Vector2f factor)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        Transformable::scale(factor);
        for (const auto& child : m_children)
        {
            child->scale(factor);
        }
    }

    void GameObject::AddChild(std::shared_ptr<GameObject> child)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_children.push_back(child);
    }

    void GameObject::RemoveChild(int id)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_children.erase(m_children.cbegin() + id);
    }

    std::vector<std::shared_ptr<GameObject>> GameObject::GetChildren() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return std::vector<std::shared_ptr<GameObject>>(m_children);
    }

    void GameObject::Update(float dt)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& child : m_children)
        {
            child->Update(dt);
        }
    }

    void GameObject::HandleInput(const sf::Event& event)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& child : m_children)
        {
            child->HandleInput(event);
        }
    }
    bool GameObject::IsPointInside(const sf::Vector2f& point) const
    {
        return m_vertices->getBounds().contains(point);
    }
}