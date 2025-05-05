#pragma once

#include "EngineDefines.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ShakEvent.h"

namespace shak
{
    class Camera;
    enum class CameraResizeBehavior;
    class ShakEngine
    {
    public:
        // Delete copy constructor and assignment operator to prevent copying
        ShakEngine(const ShakEngine&) = delete;
        ShakEngine(ShakEngine&&) = delete;
        ShakEngine& operator=(const ShakEngine&) = delete;
        ShakEngine& operator=(ShakEngine&&) = delete;

        // Static method to get the singleton instance
        static ShakEngine& GetInstance();

        void Initialize(const std::string& windowTitle = "ShakEngine");

        std::shared_ptr<Renderer> GetRenderer() const { return m_renderer; }

        template<typename T, typename... Args>
        std::shared_ptr<T> AddGameObject(Args&& ... args)
        {
            static_assert(std::is_base_of<GameObject, T>::value, "T must be a GameObject");
            const std::shared_ptr<T> tPtr = std::make_shared<T>(std::forward<Args>(args)...);
            m_sceneManager->GetActiveScene()->AddGameObject(tPtr);
            return tPtr;
        }
        void Destroy(const GameObjectPtr& gameObject);
        void Destroy(const std::string& name); //TODO: id removal and search
        inline unsigned int GetNextGameObjectId() { return m_nextGameObjectId++; }

        GameObjectPtr FindGameObjectByName(const std::string& name) const;

        template <typename T>
        std::vector<GameObjectPtr> FindGameObjectsByType() const
        {
            static_assert(std::is_base_of<GameObject, T>::value, "T must be a GameObject");
            return m_sceneManager->GetActiveScene()->FindGameObjectsByType<T>();
        }

        std::shared_ptr<shak::Camera> AddCamera(const std::string& name, const sf::FloatRect& viewport, const CameraResizeBehavior& resizeBehavior);

        template <typename T>
        void AddUIElement(const std::string& name)
        {
            static_assert(std::is_base_of<UIElement, T>::value, "T must be a UIElement");
            m_sceneManager->GetActiveScene()->AddUIElement(name, std::make_shared<T>());
        }
        void RemoveUIElement(const std::string& name);
        std::shared_ptr<UIElement> FindUIElementByName(const std::string& name) const;
        void SelectActiveUI(const std::string& name);
        void DeselectActiveUI();
        std::shared_ptr<UIElement> GetActiveUI() const;

        std::shared_ptr<ResourceManager> GetResourceManager();

        std::shared_ptr<SceneManager> GetSceneManager() const { return m_sceneManager; }
        std::shared_ptr<Scene> GetScene() const { return m_sceneManager->GetActiveScene(); }

        sf::Vector2i GetPointInScreenCoords(const sf::Vector2f& worldPos) const;
        sf::Vector2f GetMousePixelPos() const;
        /// @brief Obtains the mouse position in world coordinates. Optionally, you can specify a target view to get the mouse position in that view's coordinates.
        /// @param targetView view to convert the mouse position to. If nullptr, the default view is used.
        /// @return point in world coordinates
        sf::Vector2f GetMouseWorldPos(const std::shared_ptr<sf::View>& targetView = nullptr) const;
        sf::Vector2f GetWindowSize() const;

        void Start();

        float GetTime();

    public:
        //EVENTS
        // <old, new>
        Event<const sf::Vector2u&, const sf::Vector2u&> OnResize;

    private:
        // Private constructor to prevent instantiation
        ShakEngine();
        ~ShakEngine() = default;

    private:
        std::shared_ptr<Renderer> m_renderer;
        std::shared_ptr<sf::RenderWindow> m_window;
        sf::Vector2u m_windowSize;
        std::shared_ptr<ResourceManager> m_resourceManager;
        std::shared_ptr<SceneManager> m_sceneManager;
        sf::Clock m_clock;
        unsigned int m_nextGameObjectId;
        float m_time;
    };
}