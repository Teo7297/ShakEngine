#pragma once

#include "EngineDefines.h"
#include "GameObject.h"
#include "Renderer.h"

namespace shak
{
    class Scene
    {
    public:
        Scene(std::shared_ptr<shak::Renderer> renderer);
        virtual ~Scene() = default;

        void AddGameObject(GameObjectPtr gameObject);

        void RemoveGameObject(int id);

        GameObjectPtr FindGameObject(std::string name) const;
        GameObjectPtr FindGameObject(int id) const;

        template<typename T>
        std::vector<GameObjectPtr> FindGameObjectsByType() const
        {
            return m_root->FindChildrenByTypeRecursive<T>();
        }

        void Update(float dt);

        void Render();

        void HandleInput(const sf::Event& event);

    private:
        GameObjectPtr m_root;
        std::vector<GameObjectPtr> m_drawables;
        std::shared_ptr<shak::Renderer> m_renderer;
        bool m_awakeDone = false;
    };
}