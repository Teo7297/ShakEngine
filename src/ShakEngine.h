#pragma once

#include "EngineDefines.h"
#include "ResourceManager.h"
#include "Scene.h"

class ShakEngine
{
public:
    ShakEngine()
        : m_renderer{ std::make_shared<shak::Renderer>() },
        m_window{ nullptr },
        m_resourceManager{},
        m_scene{ m_renderer },
        m_clock{},
        m_cameras{}
    {
        m_window = m_renderer->CreateSFWindow("GAME");
        m_renderer->Start();
        m_window->setFramerateLimit(170);
    }

    ~ShakEngine() = default;
    ShakEngine(const ShakEngine&) = delete;
    ShakEngine(ShakEngine&&) = delete;
    ShakEngine& operator=(const ShakEngine&) = delete;
    ShakEngine& operator=(ShakEngine&&) = delete;

    void AddGameObject(std::shared_ptr<shak::GameObject> gameObject)
    {
        m_scene.AddGameObject(gameObject);
    }

    void RemoveGameObject(std::shared_ptr<shak::GameObject> gameObject)
    {
        m_scene.RemoveGameObject(gameObject);
    }

    shak::ResourceManager& GetResourceManager()
    {
        return m_resourceManager;
    }

    void AddCamera(std::string name, std::shared_ptr<sf::View> camera)
    {
        m_cameras[name] = camera;
    }

    void RemoveCamera(std::string name)
    {
        m_cameras.erase(name);
    }

    void Start()
    {
        while (m_window->isOpen())
        {
            while (const std::optional event = m_window->pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    m_renderer->Stop();
                    exit(0);
                }
                else if (event->is<sf::Event::Resized>())
                {
                    auto val = event->getIf<sf::Event::Resized>();
                    // camera.setSize({ (float)val->size.x, (float)val->size.y });
                }

                // else if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>())
                // {
                //     if (wheel->delta > 0)
                //     {
                //         camera.zoom(0.9f);
                //     }
                //     else
                //     {
                //         camera.zoom(1.1f);
                //     }
                // }
                // else if (event->is<sf::Event::MouseMoved>())
                // {
                //     mousePosPixel = { (float)sf::Mouse::getPosition(*window).x, window->getSize().y - (float)sf::Mouse::getPosition(*window).y }; // TODO: update mapping on resize
                //     mousePosWorld = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
                //     shader->setUniform("mouse", mousePosPixel);
                // }

                m_scene.HandleInput(event.value());
            }

            // Calculate delta time
            float dt = m_clock.restart().asSeconds();

            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            //     camera.setCenter({ camera.getCenter().x, camera.getCenter().y - 1000 * dt });
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            //     camera.setCenter({ camera.getCenter().x, camera.getCenter().y + 1000 * dt });
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            //     camera.setCenter({ camera.getCenter().x - 1000 * dt, camera.getCenter().y });
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            //     camera.setCenter({ camera.getCenter().x + 1000 * dt, camera.getCenter().y });

            // Update the active scene
            m_scene.Update(dt);

            m_scene.Render();

            for (auto& [name, camera] : m_cameras)
            {
                m_window->setView(*camera); //TODO: TEST
            }
        }
    }



private:

private:
    std::shared_ptr<shak::Renderer> m_renderer;
    std::shared_ptr<sf::RenderWindow> m_window;
    shak::ResourceManager m_resourceManager;
    shak::Scene m_scene;
    sf::Clock m_clock;
    std::unordered_map<std::string, std::shared_ptr<sf::View>> m_cameras;
};