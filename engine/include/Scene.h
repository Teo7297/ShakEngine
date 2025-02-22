#pragma once

#include "EngineDefines.h"
#include "GameObject.h"
#include "Renderer.h"
#include "QuadTree.h"

namespace shak
{
    struct RaycastHit
    {
        GameObjectPtr hitObject;
        sf::Vector2f hitPoint;
        sf::Vector2f normal;
        float distance;
    };

    class Scene
    {
    public:
        Scene(std::shared_ptr<shak::Renderer> renderer);
        virtual ~Scene() = default;

        void AddGameObject(const GameObjectPtr gameObject);
        void AddGameObjectToQuadtree(const GameObjectPtr& gameObject);

        void RemoveGameObject(int id);

        GameObjectPtr FindGameObject(const std::string& name) const;
        GameObjectPtr FindGameObject(int id) const;
        void Raycast(const sf::Vector2f& origin, const sf::Vector2f& direction, float maxDistance, std::vector<RaycastHit>& outHits, bool drawDebug = false);

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

        void CheckCollisions();

    private:
        Quadtree m_quadtree;
        GameObjectPtr m_root;
        std::vector<GameObjectPtr> m_drawables;
        std::shared_ptr<shak::Renderer> m_renderer;
        bool m_awakeDone;
    };
}