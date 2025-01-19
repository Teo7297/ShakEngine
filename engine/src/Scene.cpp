#include "Scene.h"

void shak::Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
    m_gameObjects.push_back(gameObject);
}

void shak::Scene::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
    m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), gameObject), m_gameObjects.end());
}

void shak::Scene::Update(float dt)
{
    for (auto& gameObject : m_gameObjects)
    {
        gameObject->Update(dt);
    }
}

void shak::Scene::Render()
{
    m_drawables.clear();
    for (auto& gameObject : m_gameObjects)
    {
        m_drawables.emplace_back(gameObject, nullptr);
    }
    m_renderer->PushToRenderQueue(m_drawables);
}

void shak::Scene::HandleInput(const sf::Event& event)
{
    for (auto& gameObject : m_gameObjects)
    {
        gameObject->HandleInput(event);
    }
}
