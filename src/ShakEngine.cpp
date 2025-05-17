#include "ShakEngine.h"
#include "Renderer.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "Camera.h"

#include "ShakUI/Logger_UI.h"

namespace shak
{

    // Private constructor
    ShakEngine::ShakEngine()
        : m_renderer(nullptr)
        , m_window(nullptr)
        , m_resourceManager(nullptr)
        , m_sceneManager(nullptr)
        , m_clock()
        , m_nextGameObjectId(0)
        , m_time(0.f)
    {
    }

    ShakEngine& ShakEngine::GetInstance()
    {
        static ShakEngine instance;
        return instance;
    }

    void ShakEngine::Initialize(const std::string& windowTitle)
    {
        m_renderer = std::make_shared<shak::Renderer>();
        m_window = m_renderer->CreateSFWindow(windowTitle);
        m_windowSize = m_window->getSize();
        m_sceneManager = std::make_shared<SceneManager>();
        m_resourceManager = std::make_shared<ResourceManager>();
    }

    void ShakEngine::Destroy(const GameObjectPtr& gameObject)
    {
        m_sceneManager->GetActiveScene()->RemoveGameObject(gameObject->Id);
    }

    void ShakEngine::Destroy(const std::string& name)
    {
        Destroy(FindGameObjectByName(name));
    }

    GameObjectPtr ShakEngine::FindGameObjectByName(const std::string& name) const
    {
        return m_sceneManager->GetActiveScene()->FindGameObject(name);
    }

    std::shared_ptr<shak::Camera> ShakEngine::AddCamera(const std::string& name, const sf::FloatRect& rectangle, const CameraResizeBehavior& resizeBehavior)
    {
        auto camera = m_renderer->AddCamera(name, rectangle, resizeBehavior);
        this->GetSceneManager()->GetActiveScene()->AddGameObject(std::static_pointer_cast<GameObject>(camera));
        return camera;
    }

    void ShakEngine::RemoveCamera(const std::string& name)
    {
        m_renderer->RemoveCamera(name);
        this->Destroy(name);
    }

    void ShakEngine::RemoveUIElement(const std::string& name)
    {
        m_sceneManager->GetActiveScene()->RemoveUIElement(name);
    }

    std::shared_ptr<UIElement> ShakEngine::FindUIElementByName(const std::string& name) const
    {
        return m_sceneManager->GetActiveScene()->GetUIElement(name);
    }

    void ShakEngine::SelectActiveUI(const std::string& name)
    {
        m_sceneManager->GetActiveScene()->SelectActiveUI(name);
    }

    void ShakEngine::DeselectActiveUI(const std::string& name)
    {
        m_sceneManager->GetActiveScene()->DeselectActiveUI(name);
    }

    std::vector<std::shared_ptr<UIElement>> ShakEngine::GetActiveUIs() const
    {
        return m_sceneManager->GetActiveScene()->GetActiveUIs();
    }

    std::shared_ptr<ResourceManager> ShakEngine::GetResourceManager()
    {
        return m_resourceManager;
    }

    std::shared_ptr<Logger_UI> ShakEngine::GetLogger()
    {
        if(!FindUIElementByName("Logger_UI"))
        {
            this->AddUIElement<shak::Logger_UI>("Logger_UI");
            this->SelectActiveUI("Logger_UI");
        }

        return std::static_pointer_cast<Logger_UI>(FindUIElementByName("Logger_UI"));
    }

    sf::Vector2i ShakEngine::GetPointInScreenCoords(const sf::Vector2f& worldPos) const
    {
        return m_window->mapCoordsToPixel(worldPos);
    }

    sf::Vector2f ShakEngine::GetMousePixelPos() const
    {
        return { static_cast<float>(sf::Mouse::getPosition(*m_window).x), static_cast<float>(m_window->getSize().y - sf::Mouse::getPosition(*m_window).y) };
    }

    sf::Vector2f ShakEngine::GetMouseWorldPos(const std::shared_ptr<sf::View>& targetView) const
    {
        if(targetView)
            return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window), *targetView.get());
        else
            return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
    }

    sf::Vector2f ShakEngine::GetWindowSize() const
    {
        return { static_cast<float>(m_window->getSize().x), static_cast<float>(m_window->getSize().y) };
    }

    //! ////////////////////// WINDOW SETTINGS //////////////////////

    void ShakEngine::SetIcon(const sf::Image& image)
    {
        m_window->setIcon(image.getSize(), image.getPixelsPtr());
    }

    void ShakEngine::SetIcon(const sf::Vector2u& size, const std::uint8_t* pixels)
    {
        m_window->setIcon(size, pixels);
    }

    void ShakEngine::SetWindowTitle(const std::string& title)
    {
        m_window->setTitle(title);
    }

    void ShakEngine::SetWindowFrameLimit(unsigned int limit)
    {
        m_window->setFramerateLimit(limit);
    }

    //! ////////////////////// WINDOW LOOP //////////////////////

    void ShakEngine::Start()
    {
        while(m_window->isOpen())
        {
            std::shared_ptr<shak::Scene> activeScene = m_sceneManager->GetActiveScene();

            while(const std::optional event = m_window->pollEvent())
            {
                // Process ImGui events
                ImGui::SFML::ProcessEvent(*m_window, *event);

                if(event->is<sf::Event::Closed>())
                {
                    m_renderer->CloseWindow();
                    exit(0);
                }
                else if(event->is<sf::Event::Resized>())
                {
                    auto newWindowSize = event->getIf<sf::Event::Resized>()->size;
                    OnResize(m_windowSize, newWindowSize);
                    m_windowSize = newWindowSize;
                }

                activeScene->HandleInput(event.value());
            }

            // Calculate delta time
            float dt = m_clock.restart().asSeconds();
            m_time += dt;

            activeScene->ForwardAwake();

            activeScene->TryInitActiveUI();

            activeScene->Update(dt);

            activeScene->CheckCollisions();

            // Update mouse and touch position on screen before drawing the UI
            ImGui::SFML::Update(*m_window, sf::seconds(dt));

            activeScene->UpdateUI(dt);

            activeScene->DrawUI();

            activeScene->Render();

            activeScene->Cleanup();

            // Check if we need to load a new scene, we do this after the render call to avoid breaking logic
            m_sceneManager->TryActivateQueuedScene();
        }
    }

    float ShakEngine::GetTime()
    {
        return m_time;
    }
}

