#pragma once

#include "EngineDefines.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Camera.h"

namespace shak
{
    class ShakEngine
    {
    public:
        // Delete copy constructor and assignment operator to prevent copying
        ShakEngine(const ShakEngine&) = delete;
        ShakEngine(ShakEngine&&) = delete;
        ShakEngine& operator=(const ShakEngine&) = delete;
        ShakEngine& operator=(ShakEngine&&) = delete;

        // Static method to get the singleton instance
        static ShakEngine& GetInstance()
        {
            static ShakEngine instance;
            return instance;
        }

        void Initialize(const std::string& windowTitle = "ShakEngine");

        void AddGameObject(const GameObjectPtr gameObject);
        void Destroy(const GameObjectPtr& gameObject);
        inline unsigned int GetNextGameObjectId() { return m_nextGameObjectId++; }

        GameObjectPtr FindGameObjectByName(std::string name) const;

        template <typename T>
        std::vector<GameObjectPtr> FindGameObjectsByType() const
        {
            return m_scene->FindGameObjectsByType<T>();
        }

        ResourceManager& GetResourceManager();
        std::shared_ptr<Scene> GetScene() const { return m_scene; }

        void AddCamera(std::string name, std::shared_ptr<Camera> camera);

        std::shared_ptr<Camera> GetCamera(std::string name) const;

        void RemoveCamera(std::string name);

        sf::Vector2f GetMousePixelPos() const;
        sf::Vector2f GetMouseWorldPos() const;
        sf::Vector2f GetWindowSize() const;

        void Start();

        void TestQuadtree()
        {
            auto c = m_cameras.at("camera1")->GetView();
            m_scene->TestQuadtree({ c->getCenter() - sf::Vector2f{1920.f, 1080.f} / 2.f, {1920.f, 1080.f} });
        }

    private:
        // Private constructor to prevent instantiation
        ShakEngine() = default;
        ~ShakEngine() = default;

    private:
        std::shared_ptr<Renderer> m_renderer;
        std::shared_ptr<sf::RenderWindow> m_window;
        ResourceManager m_resourceManager;
        std::shared_ptr<Scene> m_scene;
        sf::Clock m_clock;
        std::unordered_map<std::string, std::shared_ptr<Camera>> m_cameras;
        unsigned int m_nextGameObjectId = 0;
    };
}