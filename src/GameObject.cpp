#include "GameObject.h"

namespace shak
{
    GameObject::GameObject(sf::VertexArray va)
        : m_vertices(va)
    {
        if (!m_texture.loadFromFile("textures/bricks.jpg"))
        {
            std::cerr << "Failed to load texture" << std::endl;
        }
    }

    void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= this->getTransform();
        states.texture = &m_texture;
        states.shader = m_shader.get();
        target.draw(m_vertices, states);

        for (const auto& child : m_children)
        {
            child->draw(target, states);
        }
    }

    void GameObject::Render(std::vector<shak::Drawable>& drawables)
    {
        for (auto& child : m_children)
        {
            drawables.emplace_back(child, nullptr);
            child->Render(drawables);
        }
    }

    void GameObject::AddChild()
    {
        m_children.push_back(std::make_shared<GameObject>(sf::VertexArray()));
    }

    void GameObject::RemoveChild(std::shared_ptr<GameObject> child)
    {
        m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
    }

    void GameObject::SetShader(const std::shared_ptr<sf::Shader> shader)
    {
        m_shader = shader;
    }

    void GameObject::Update(float dt)
    {
        for (const auto& child : m_children)
        {
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
}