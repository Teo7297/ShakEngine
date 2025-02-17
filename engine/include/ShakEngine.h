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

        template<typename T, typename... Args>
        std::shared_ptr<T> AddGameObject(Args&& ... args)
        {
            const std::shared_ptr<T> tPtr = std::make_shared<T>(std::forward<Args>(args)...);
            m_scene->AddGameObject(tPtr);
            return tPtr;
        }
        void Destroy(const GameObjectPtr& gameObject);
        inline unsigned int GetNextGameObjectId() { return m_nextGameObjectId++; }

        GameObjectPtr FindGameObjectByName(const std::string& name) const;

        template <typename T>
        std::vector<GameObjectPtr> FindGameObjectsByType() const
        {
            return m_scene->FindGameObjectsByType<T>();
        }

        ResourceManager& GetResourceManager();
        std::shared_ptr<Scene> GetScene() const { return m_scene; }

        void AddCamera(const std::string& name, const std::shared_ptr<Camera>& camera);

        std::shared_ptr<Camera> GetCamera(const std::string& name) const;

        void RemoveCamera(const std::string& name);

        sf::Vector2f GetMousePixelPos() const;
        sf::Vector2f GetMouseWorldPos() const;
        sf::Vector2f GetWindowSize() const;

        void Start();

        float GetTime();

    private:
        // Private constructor to prevent instantiation
        ShakEngine();
        ~ShakEngine() = default;

    private:
        std::shared_ptr<Renderer> m_renderer;
        std::shared_ptr<sf::RenderWindow> m_window;
        ResourceManager m_resourceManager;
        std::shared_ptr<Scene> m_scene;
        sf::Clock m_clock;
        std::unordered_map<std::string, std::shared_ptr<Camera>> m_cameras;
        unsigned int m_nextGameObjectId;
        float m_time;
    };
}