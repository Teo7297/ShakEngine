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

        void AddGameObject(std::shared_ptr<GameObject> gameObject);

        void RemoveGameObject(int id);

        std::shared_ptr<GameObject> FindGameObject(std::string name) const;
        std::shared_ptr<GameObject> FindGameObject(int id) const;

        template<typename T>
        std::vector<std::shared_ptr<GameObject>> FindGameObjectsByType() const
        {
            return m_root->FindChildrenByTypeRecursive<T>();
        }

        void Update(float dt);

        void Render();

        void HandleInput(const sf::Event& event);

    private:
        std::shared_ptr<GameObject> m_root;
        std::vector<shak::Drawable> m_drawables;
        std::shared_ptr<shak::Renderer> m_renderer;
        bool m_awakeDone = false;
    };
}