#include "ShakEngine.h"
#include "Renderer.h"
#include "imgui.h"
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

            m_scene->CheckCollisions();

            m_scene->Update(dt);

            // TEST
            ImGui::SFML::Update(*m_window, sf::seconds(dt));

            // Before creating the window, tell ImGui where you want it:
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            // Push style colors for transparent background and borders
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));  // Fully transparent background
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));  // Fully transparent borders

            ImGui::Begin("HUD Overlay", nullptr, ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoBackground);

            // Top left region (for example, player stats)
            ImGui::BeginChild("TopLeft", ImVec2(300, 150), false);
            ImGui::Text("Health: 100");
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("This is the tooltip for the health text");
                ImGui::EndTooltip();
            }
            ImGui::Text("Energy: 100");
            // Add more widgets...
            ImGui::EndChild();

            // Top right region (for example, minimap)
            ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 310); // 300 width + margin
            ImGui::BeginChild("TopRight", ImVec2(300, 300), false);
            ImGui::Text("Minimap");
            // Add minimap widget...
            ImGui::EndChild();

            // Bottom left region (inventory, etc.)
            ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 150);
            ImGui::BeginChild("BottomLeft", ImVec2(400, 150), false);
            ImGui::Text("Inventory");
            // Add inventory widgets...
            ImGui::EndChild();

            ImGui::End();
            ImGui::PopStyleColor(2);

            // ImGui::ShowDemoWindow(); // Ctrl + Tab to focus the demo window if behind HUD

            m_scene->Render();

            m_scene->Cleanup();
        }
    }

    float ShakEngine::GetTime()
    {
        return m_time;
    }
}

