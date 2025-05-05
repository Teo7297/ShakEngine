#include "Renderer.h"
#include "Camera.h"
#include "imgui.h"
#include "imgui-SFML.h"

namespace shak
{
    Renderer::Renderer()
        : m_isRunning(true)
        , m_window(nullptr)
        , m_cameras()
        , m_clearColor(2, 38, 46)
    {
    }

    std::shared_ptr<sf::RenderWindow> Renderer::CreateSFWindow(const std::string& title)
    {
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT }), title);
        if (!m_window->setActive(true))
            std::cerr << "[Render thread] OpenGL context could not be activated" << std::endl;
        m_window->setVerticalSyncEnabled(false);
        m_window->setFramerateLimit(170);

        if (!ImGui::SFML::Init(*m_window))
            std::cerr << "[Renderer] Could not initialize ImGui::SFML" << std::endl;

        return m_window;
    }

    void Renderer::CloseWindow()
    {
        m_window->close();
        ImGui::SFML::Shutdown();
    }

    std::shared_ptr<Camera> Renderer::AddCamera(const std::string& name, const sf::FloatRect& rectangle, const CameraResizeBehavior& resizeBehavior)
    {
        auto camera = std::make_shared<shak::Camera>(rectangle, resizeBehavior);
        camera->Name = name;
        m_cameras[name] = camera;
        return camera;
    }

    void Renderer::RemoveCamera(const std::string& name)
    {
        m_cameras.erase(name);
    }

    void Renderer::Render(const std::vector<GameObjectPtr>& drawables)
    {
        m_window->clear(m_clearColor);

        // glDisable(GL_BLEND);

        // Render scene
        if (!m_cameras.empty())
        {
            for (const auto& camera : m_cameras)
            {
                m_window->setView(*camera.second->GetView());
                Draw(drawables);
            }
        }
        else
            Draw(drawables);

        // Render GUI
        ImGui::SFML::Render(*m_window);

        // Draw on screen the rendered frame
        m_window->display();
    }

    void Renderer::Draw(const std::vector<GameObjectPtr>& drawables)
    {
        for (const auto& drawable : drawables)
        {
            m_window->draw(*drawable);
        }
    }
}