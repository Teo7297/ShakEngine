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
        GameObject(sf::VertexArray va);
        virtual ~GameObject() = default;

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

        void Render(std::vector<shak::Drawable>& drawables);

        void AddChild();

        void RemoveChild(std::shared_ptr<GameObject> child);

        void SetShader(const std::shared_ptr<sf::Shader> shader);

        virtual void Update(float dt);

        virtual void HandleInput(const sf::Event& event);

    private:
        sf::VertexArray m_vertices;
        sf::Texture m_texture;
        std::shared_ptr<sf::Shader> m_shader = nullptr;
        std::vector<std::shared_ptr<GameObject>> m_children;
    };
}