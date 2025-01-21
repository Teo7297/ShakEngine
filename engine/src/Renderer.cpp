#include "Renderer.h"

namespace shak
{
    Renderer::Renderer()
    {
    }

    Renderer::~Renderer()
    {
    }

    std::shared_ptr<sf::RenderWindow> Renderer::CreateSFWindow(const std::string& title)
    {
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT }), title);
        if (!m_window->setActive(true))
            std::cerr << "[Render thread] OpenGL context could not be activated" << std::endl;
        m_window->setVerticalSyncEnabled(false);
        m_window->setFramerateLimit(170);
        return m_window;
    }

    void Renderer::CloseWindow()
    {
        m_window->close();
    }

    void Renderer::AddCamera(const std::string& name, std::shared_ptr<sf::View> camera)
    {
        m_cameras[name] = camera;
    }

    void Renderer::RemoveCamera(const std::string& name)
    {
        m_cameras.erase(name);
    }

    void Renderer::Render(const std::vector<Drawable>& drawables)
    {
        m_window->clear(m_clearColor);

        // glDisable(GL_BLEND);

        if (!m_cameras.empty())
        {
            for (const auto& camera : m_cameras)
            {
                m_window->setView(*camera.second);
                Draw(drawables);
            }
        }
        else
            Draw(drawables);

        m_window->display();
    }

    void Renderer::Draw(const std::vector<Drawable>& drawables)
    {
        for (const auto& drawable : drawables)
        {
            if (drawable.renderStates)
                m_window->draw(*drawable.drawable, *drawable.renderStates);
            else
                m_window->draw(*drawable.drawable);
        }
    }
}