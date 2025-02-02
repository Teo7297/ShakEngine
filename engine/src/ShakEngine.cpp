#include "ShakEngine.h"
#include "Renderer.h"

namespace shak
{

    void ShakEngine::Initialize(const std::string& windowTitle)
    {
        m_renderer = std::make_shared<shak::Renderer>();
        m_window = m_renderer->CreateSFWindow(windowTitle);
        m_scene = std::make_shared<Scene>(m_renderer);
    }

    void ShakEngine::AddGameObject(const GameObjectPtr gameObject)
    {
        m_scene->AddGameObject(gameObject);
    }

    void ShakEngine::Destroy(const GameObjectPtr& gameObject)
    {
        gameObject->OnDestroy();
        m_scene->RemoveGameObject(gameObject->Id);
    }

    GameObjectPtr ShakEngine::FindGameObjectByName(std::string name) const
    {
        return m_scene->FindGameObject(name);
    }

    shak::ResourceManager& ShakEngine::GetResourceManager()
    {
        return m_resourceManager;
    }

    void ShakEngine::AddCamera(std::string name, std::shared_ptr<shak::Camera> camera)
    {
        m_cameras[name] = camera;
        m_renderer->AddCamera(name, camera->GetView());
    }

    std::shared_ptr<shak::Camera> ShakEngine::GetCamera(std::string name) const
    {
        if (m_cameras.find(name) == m_cameras.end())
            return nullptr;
        return m_cameras.at(name);
    }

    void ShakEngine::RemoveCamera(std::string name)
    {
        m_cameras.erase(name);
        m_renderer->RemoveCamera(name);
    }

    sf::Vector2f ShakEngine::GetMousePixelPos() const
    {
        return { (float)sf::Mouse::getPosition(*m_window).x, m_window->getSize().y - (float)sf::Mouse::getPosition(*m_window).y };
    }

    sf::Vector2f ShakEngine::GetMouseWorldPos() const
    {
        return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
    }

    sf::Vector2f ShakEngine::GetWindowSize() const
    {
        return { (float)m_window->getSize().x, (float)m_window->getSize().y };
    }

    void ShakEngine::Start()
    {
        while (m_window->isOpen())
        {
            while (const std::optional event = m_window->pollEvent())
            {
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

            // Update the active scene
            m_scene->Update(dt);

            m_scene->Render();
        }
    }
}

