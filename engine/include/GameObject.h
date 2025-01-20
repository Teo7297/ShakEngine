#pragma once

#include "EngineDefines.h"

namespace shak
{
    class GameObject;
    struct Drawable
    {
        Drawable(std::shared_ptr<sf::Drawable> d, std::shared_ptr<sf::RenderStates> s) : drawable{ d }, renderStates{ s } {}
        ~Drawable() {}
        std::shared_ptr<sf::Drawable> drawable;
        std::shared_ptr<sf::RenderStates> renderStates;
    };

    class GameObject : public sf::Transformable, public sf::Drawable
    {
    public:
        GameObject() = default;
        GameObject(std::shared_ptr<sf::VertexArray> va, std::shared_ptr<sf::Texture> texture = nullptr);
        virtual ~GameObject() = default;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

        virtual void move(sf::Vector2f offset);

        void rotate(sf::Angle angle);

        void scale(sf::Vector2f factor);

        void AddChild(std::shared_ptr<GameObject> child);

        void RemoveChild(int id);

        inline void SetActive(bool active) { m_active = active; }
        inline bool IsActive() const { return m_active; }

        /// @brief Obtain a copy of the children vector of this GameObject
        /// @return std::vector<std::shared_ptr<GameObject>>
        std::vector<std::shared_ptr<GameObject>> GetChildren() const;

        inline void SetShader(const std::shared_ptr<sf::Shader> shader) { m_shader = shader; }
        inline std::shared_ptr<sf::Shader> GetShader() { return m_shader; }

        inline void SetVertexArray(const std::shared_ptr<sf::VertexArray> va) { m_vertices = va; }
        inline std::shared_ptr<sf::VertexArray> GetVertexArray() { return m_vertices; }

        inline void SetTexture(const std::shared_ptr<sf::Texture> texture) { m_texture = texture; }
        inline std::shared_ptr<sf::Texture> GetTexture() { return m_texture; }

        virtual void Update(float dt);

        virtual void HandleInput(const sf::Event& event);

        // Physics
        bool IsPointInside(const sf::Vector2f& point) const;

    protected:
        std::shared_ptr<sf::VertexArray> m_vertices = nullptr;
        std::shared_ptr<sf::Texture> m_texture = nullptr;
        std::shared_ptr<sf::Shader> m_shader = nullptr;
        std::vector<std::shared_ptr<GameObject>> m_children;
        bool m_active = true;
    };
}