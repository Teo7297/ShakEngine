#pragma once

#include "EngineDefines.h"
#include "GameObject.h"
#include "Renderer.h"
#include "QuadTree.h"

namespace shak
{
    class Scene
    {
    public:
        Scene(std::shared_ptr<shak::Renderer> renderer);
        virtual ~Scene() = default;

        void AddGameObject(const GameObjectPtr& gameObject);
        void AddGameObjectToQuadtree(const GameObjectPtr& gameObject);

        void RemoveGameObject(int id);

        GameObjectPtr FindGameObject(const std::string& name) const;
        GameObjectPtr FindGameObject(int id) const;

        template<typename T>
        std::vector<GameObjectPtr> FindGameObjectsByType() const
        {
            return m_root->FindChildrenByTypeRecursive<T>();
        }

        void ForwardAwake();

        void Update(float dt);

        void Render();

        void Cleanup();

        void HandleInput(const sf::Event& event);

        void TestQuadtree(sf::FloatRect area);

    private:
        Quadtree m_quadtree;
        GameObjectPtr m_root;
        std::vector<GameObjectPtr> m_drawables;
        std::shared_ptr<shak::Renderer> m_renderer;
        bool m_awakeDone = false;
    };
}