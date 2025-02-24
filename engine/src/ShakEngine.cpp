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
        , m_resourceManager()
        , m_scene(nullptr)
        , m_clock()
        , m_cameras()
        , m_nextGameObjectId(0)
        , m_time(0.f)
    {
    }

    void ShakEngine::Initialize(const std::string& windowTitle)
    {
        m_renderer = std::make_shared<shak::Renderer>();
        m_window = m_renderer->CreateSFWindow(windowTitle);
        m_scene = std::make_shared<Scene>(m_renderer);
    }

    void ShakEngine::Destroy(const GameObjectPtr& gameObject)
    {
        gameObject->OnDestroy();
        m_scene->RemoveGameObject(gameObject->Id);
    }

    GameObjectPtr ShakEngine::FindGameObjectByName(const std::string& name) const
    {
        return m_scene->FindGameObject(name);
    }

    void ShakEngine::RemoveUIElement(const std::string& name)
    {
        m_scene->RemoveUIElement(name);
    }

    std::shared_ptr<UIElement> ShakEngine::FindUIElementByName(const std::string& name) const
    {
        return m_scene->GetUIElement(name);
    }

    void ShakEngine::SelectActiveUI(const std::string& name)
    {
        m_scene->SelectActiveUI(name);
    }

    void ShakEngine::DeselectActiveUI()
    {
        m_scene->DeselectActiveUI();
    }

    std::shared_ptr<UIElement> ShakEngine::GetActiveUI() const
    {
        return m_scene->GetActiveUI();
    }

    shak::ResourceManager& ShakEngine::GetResourceManager()
    {
        return m_resourceManager;
    }

    void ShakEngine::AddCamera(const std::string& name, const std::shared_ptr<shak::Camera>& camera)
    {
        m_cameras[name] = camera;
        m_renderer->AddCamera(name, camera->GetView());
    }

    std::shared_ptr<shak::Camera> ShakEngine::GetCamera(const std::string& name) const
    {
        if (!m_cameras.contains(name))
            return nullptr;
        return m_cameras.at(name);
    }

    void ShakEngine::RemoveCamera(const std::string& name)
    {
        m_cameras.erase(name);
        m_renderer->RemoveCamera(name);
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
                    auto val = event->getIf<sf::Event::Resized>();
                    for (auto& [name, camera] : m_cameras)
                    {
                        //TODO: make this more flexible, this now only works with horizontal split screen
                        camera->SetSize({ (float)val->size.x / m_cameras.size(), (float)val->size.y });
                    }
                }

                m_scene->HandleInput(event.value());
            }

            // Calculate delta time
            float dt = m_clock.restart().asSeconds();
            m_time += dt;

            m_scene->ForwardAwake();

            m_scene->TryInitActiveUI();

            m_scene->CheckCollisions();

            m_scene->Update(dt);

            // TEST
            // Update mouse and touch position on screen before drawing the UI
            ImGui::SFML::Update(*m_window, sf::seconds(dt));

            m_scene->DrawUI();

            m_scene->Render();

            m_scene->Cleanup();
        }
    }

    float ShakEngine::GetTime()
    {
        return m_time;
    }
}

