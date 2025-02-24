#include "ShakEngine.h"
#include "Renderer.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"

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

    void ShakEngine::Initialize(const std::string& windowTitle)
    {
        m_renderer = std::make_shared<shak::Renderer>();
        m_window = m_renderer->CreateSFWindow(windowTitle);
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

    void ShakEngine::DeselectActiveUI()
    {
        m_sceneManager->GetActiveScene()->DeselectActiveUI();
    }

    std::shared_ptr<UIElement> ShakEngine::GetActiveUI() const
    {
        return m_sceneManager->GetActiveScene()->GetActiveUI();
    }

    std::shared_ptr<ResourceManager> ShakEngine::GetResourceManager()
    {
        return m_resourceManager;
    }

    sf::Vector2i ShakEngine::GetPointInScreenCoords(const sf::Vector2f& worldPos) const
    {
        return m_window->mapCoordsToPixel(worldPos);
    }

    sf::Vector2f ShakEngine::GetMousePixelPos() const
    {
        return { static_cast<float>(sf::Mouse::getPosition(*m_window).x), static_cast<float>(m_window->getSize().y - sf::Mouse::getPosition(*m_window).y) };
    }

    sf::Vector2f ShakEngine::GetMouseWorldPos() const
    {
        return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
    }

    sf::Vector2f ShakEngine::GetWindowSize() const
    {
        return { static_cast<float>(m_window->getSize().x), static_cast<float>(m_window->getSize().y) };
    }

    void ShakEngine::Start()
    {
        while (m_window->isOpen())
        {
            while (const std::optional event = m_window->pollEvent())
            {
                // Process ImGui events
                ImGui::SFML::ProcessEvent(*m_window, *event);

                if (event->is<sf::Event::Closed>())
                {
                    m_renderer->CloseWindow();
                    exit(0);
                }
                else if (event->is<sf::Event::Resized>())
                {
                    OnResize(event->getIf<sf::Event::Resized>()->size);
                }

                m_sceneManager->GetActiveScene()->HandleInput(event.value());
            }

            // Calculate delta time
            float dt = m_clock.restart().asSeconds();
            m_time += dt;

            m_sceneManager->GetActiveScene()->ForwardAwake();

            m_sceneManager->GetActiveScene()->TryInitActiveUI();

            m_sceneManager->GetActiveScene()->CheckCollisions();

            m_sceneManager->GetActiveScene()->Update(dt);

            // TEST
            // Update mouse and touch position on screen before drawing the UI
            ImGui::SFML::Update(*m_window, sf::seconds(dt));

            m_sceneManager->GetActiveScene()->DrawUI();

            m_sceneManager->GetActiveScene()->Render();

            m_sceneManager->GetActiveScene()->Cleanup();
        }
    }

    float ShakEngine::GetTime()
    {
        return m_time;
    }
}

