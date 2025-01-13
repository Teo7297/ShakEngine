#pragma once

#include "EngineDefines.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Camera.h"

class ShakEngine
{
public:
    ShakEngine();
    ~ShakEngine() = default;
    ShakEngine(const ShakEngine&) = delete;
    ShakEngine(ShakEngine&&) = delete;
    ShakEngine& operator=(const ShakEngine&) = delete;
    ShakEngine& operator=(ShakEngine&&) = delete;

    void AddGameObject(std::shared_ptr<shak::GameObject> gameObject);

    void RemoveGameObject(std::shared_ptr<shak::GameObject> gameObject);

    shak::ResourceManager& GetResourceManager();

    void AddCamera(std::string name, std::shared_ptr<shak::Camera> camera);

    std::shared_ptr<shak::Camera> GetCamera(std::string name) const;

    void RemoveCamera(std::string name);

    sf::Vector2f GetMousePixelPos() const;

    void Start();

private:

private:
    std::shared_ptr<shak::Renderer> m_renderer;
    std::shared_ptr<sf::RenderWindow> m_window;
    shak::ResourceManager m_resourceManager;
    shak::Scene m_scene;
    sf::Clock m_clock;
    std::unordered_map<std::string, std::shared_ptr<shak::Camera>> m_cameras;
};