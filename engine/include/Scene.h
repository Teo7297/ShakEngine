#pragma once

#include "EngineDefines.h"
#include "GameObject.h"
#include "Renderer.h"

namespace shak
{
    class Scene
    {
    public:
        Scene(std::shared_ptr<shak::Renderer> renderer) : m_renderer(renderer) {}
        virtual ~Scene() = default;

        void AddGameObject(std::shared_ptr<GameObject> gameObject);

        void RemoveGameObject(std::shared_ptr<GameObject> gameObject);

        std::shared_ptr<GameObject> FindGameObjectByName(std::string name) const;

        void Update(float dt);

        void Render();

        void HandleInput(const sf::Event& event);

    private:
        std::vector<std::shared_ptr<GameObject>> m_gameObjects;
        std::vector<shak::Drawable> m_drawables;
        std::shared_ptr<shak::Renderer> m_renderer;
        bool m_awakeDone = false;
    };
}