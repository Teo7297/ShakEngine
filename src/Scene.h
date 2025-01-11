#pragma once

#include "EngineDefines.h"
#include "GameObject.h"
#include "Renderer.h"

namespace shak
{
    class Scene
    {
    public:
        Scene(std::shared_ptr<shak::Renderer> renderer) : m_renderer(renderer)
        {
        }
        virtual ~Scene() = default;

        void AddGameObject(std::shared_ptr<GameObject> gameObject)
        {
            m_gameObjects.push_back(gameObject);
        }

        void RemoveGameObject(std::shared_ptr<GameObject> gameObject)
        {
            m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), gameObject), m_gameObjects.end());
        }

        void Update(float dt)
        {
            for (auto& gameObject : m_gameObjects)
            {
                gameObject->Update(dt);
            }
        }

        void Render()
        {
            m_drawables.clear();
            for (auto& gameObject : m_gameObjects)
            {
                m_drawables.emplace_back(gameObject, nullptr);
                gameObject->Render(m_drawables);
            }
            m_renderer->PushToRenderQueue(m_drawables);
        }

        void HandleInput(const sf::Event& event)
        {
            for (auto& gameObject : m_gameObjects)
            {
                gameObject->HandleInput(event);
            }
        }

    private:
        std::vector<std::shared_ptr<GameObject>> m_gameObjects;
        std::vector<shak::Drawable> m_drawables;
        std::shared_ptr<shak::Renderer> m_renderer;
    };
}