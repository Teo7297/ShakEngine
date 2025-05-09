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
        , m_blendingEnabled(true)
    {
    }

    std::shared_ptr<sf::RenderWindow> Renderer::CreateSFWindow(const std::string& title)
    {
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT }), title);
        if(!m_window->setActive(true))
            std::cerr << "[Render thread] OpenGL context could not be activated" << std::endl;
        m_window->setVerticalSyncEnabled(false);
        m_window->setFramerateLimit(170);

        if(!ImGui::SFML::Init(*m_window))
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
        m_cameras.emplace_back(camera);
        return camera;
    }

    void Renderer::RemoveCamera(const std::string& name)
    {
        for(auto it = m_cameras.begin(); it != m_cameras.end(); ++it)
        {
            if((*it)->Name == name)
            {
                m_cameras.erase(it);
                return;
            }
        }
    }

    void Renderer::Render(const std::vector<GameObjectPtr>& drawables)
    {
        m_window->clear(m_clearColor);

        // Blending is enabled by default, we need to disable it at the beginning of the frame if we don't want it
        if(!m_blendingEnabled)
            glDisable(GL_BLEND);

        // tmp objects
        std::shared_ptr<sf::RenderTexture> tmpRenderTexturePtr = nullptr;
        std::vector<std::shared_ptr<GameObject>> renderTextureDrawables;

        // Render scene
        if(!m_cameras.empty())
        {
            bool alreadySorted = false;

            for(auto& camera : m_cameras)
            {
                if(!alreadySorted && camera->IsPriorityChanged())
                {
                    std::sort(m_cameras.begin(), m_cameras.end(), [](const std::shared_ptr<Camera>& a, const std::shared_ptr<Camera>& b) {
                        return a->GetPriority() < b->GetPriority();
                        });
                    alreadySorted = true;
                }

                camera->ResetPriorityChanged();
            }

            for(const auto& camera : m_cameras)
            {
                if(!camera->IsDrawEnabled())
                    continue;

                auto renderTarget = camera->GetRenderTarget();

                switch(renderTarget)
                {
                case CameraRenderTarget::Window:
                    m_window->setView(*camera->GetView());
                    for(const auto& drawable : drawables)
                    {
                        if(drawable->IsRenderTarget())
                            continue;
                        m_window->draw(*drawable);
                    }
                    break;

                case CameraRenderTarget::RenderTexture:
                    tmpRenderTexturePtr = camera->GetRenderTexture();
                    for(const auto& drawable : drawables)
                    {
                        if(drawable->IsRenderTarget())
                        {
                            renderTextureDrawables.emplace_back(drawable);
                            continue;
                        }
                        else
                            tmpRenderTexturePtr->draw(*drawable);
                    }
                    tmpRenderTexturePtr->display();
                    m_window->setView(*camera->GetView());

                    for(const auto& drawable : renderTextureDrawables)
                        m_window->draw(*drawable);

                    renderTextureDrawables.clear();
                    break;

                default:
                    break;
                }
            }
        }
        else
            std::cerr << "[Renderer] No cameras available to render" << std::endl;

        // Render GUI
        ImGui::SFML::Render(*m_window);

        // Draw on screen the rendered frame
        m_window->display();
    }
}