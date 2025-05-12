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

    class ShakEngine;
    class UIElement;
    class UIManager;
    class Scene
    {
    public:
        Scene(const std::string& name);
        virtual ~Scene() = default;

        std::string GetName() const { return m_name; }

        void InternalInit();
        virtual void Init() {};
        void InternalClear();
        virtual void Clear() {};

        // GAMEOBJECTS MANAGEMENT
        void AddGameObject(const GameObjectPtr gameObject);
        void AddGameObjectToQuadtree(const GameObjectPtr& gameObject);
        void RemoveGameObject(int id);

        GameObjectPtr FindGameObject(const std::string& name) const;
        GameObjectPtr FindGameObject(int id) const;

        template<typename T>
        std::vector<GameObjectPtr> FindGameObjectsByType() const
        {
            return m_root->FindChildrenByTypeRecursive<T>();
        }

        // UI MANAGEMENT
        void AddUIElement(const std::string& name, const std::shared_ptr<UIElement>& element);
        void RemoveUIElement(const std::string& name);
        std::shared_ptr<UIElement> GetUIElement(const std::string& name);
        void SelectActiveUI(const std::string& name);
        void DeselectActiveUI(const std::string& name);
        std::vector<std::shared_ptr<UIElement>> GetActiveUIs();

        // CASTING
        void Raycast(const sf::Vector2f& origin, const sf::Vector2f& direction, float maxDistance, std::vector<RaycastHit>& outHits, bool drawDebug = false);
        void Circlecast(const sf::Vector2f& center, float radius, std::vector<RaycastHit>& outHits, bool drawDebug = false);

        // GAME LOOP
        void ForwardAwake();
        void TryInitActiveUI();
        void Update(float dt);
        void Render();
        void Cleanup();
        void HandleInput(const sf::Event& event);
        void CheckCollisions();
        void UpdateUI(float dt);
        void DrawUI();

    protected:
        ShakEngine* m_engine;
        Quadtree m_quadtree;
        GameObjectPtr m_root;
        std::vector<GameObjectPtr> m_drawables;
        std::shared_ptr<shak::Renderer> m_renderer;
        bool m_awakeDone;
        std::shared_ptr<shak::UIManager> m_ui;
        std::string m_name;
    };
}