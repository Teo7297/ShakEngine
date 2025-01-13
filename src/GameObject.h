#pragma once

#include "EngineDefines.h"

namespace shak
{
    class GameObject;
    struct Drawable
    {
        Drawable(std::shared_ptr<GameObject> d, std::shared_ptr<sf::RenderStates> s) : drawable{ d }, renderStates{ s } {}
        ~Drawable() {}
        std::shared_ptr<GameObject> drawable;
        std::shared_ptr<sf::RenderStates> renderStates;
    };

    class GameObject : public sf::Transformable, public sf::Drawable
    {
    public:
        GameObject(std::shared_ptr<sf::VertexArray> va, std::shared_ptr<sf::Texture> texture = nullptr);
        virtual ~GameObject() = default;

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

        virtual void move(sf::Vector2f offset);

        void rotate(sf::Angle angle);

        void scale(sf::Vector2f factor);

        void AddChild(std::shared_ptr<GameObject> child);

        void RemoveChild(int id);

        /// @brief Obtain a copy of the children vector of this GameObject
        /// @return std::vector<std::shared_ptr<GameObject>>
        std::vector<std::shared_ptr<GameObject>> GetChildren() const;

        void SetShader(const std::shared_ptr<sf::Shader> shader);

        virtual void Update(float dt);

        virtual void HandleInput(const sf::Event& event);

    protected:
        std::shared_ptr<sf::VertexArray> m_vertices;
        std::shared_ptr<sf::Texture> m_texture;
        std::shared_ptr<sf::Shader> m_shader = nullptr;
        std::vector<std::shared_ptr<GameObject>> m_children;
        mutable std::mutex m_mutex;
    };
}