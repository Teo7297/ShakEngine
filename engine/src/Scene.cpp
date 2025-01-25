#include "Scene.h"

void shak::Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
    m_gameObjects.push_back(gameObject);
}

void shak::Scene::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
    m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), gameObject), m_gameObjects.end());
}

std::shared_ptr<shak::GameObject> shak::Scene::FindGameObjectByName(std::string name) const
{
    for (const auto& gameObject : m_gameObjects)
    {
        if (gameObject->Name == name)
            return gameObject;
    }
    return nullptr;
}

void shak::Scene::Update(float dt)
{
    if (!m_awakeDone)
    {
        for (auto& gameObject : m_gameObjects)
            if (gameObject->IsActive())
                gameObject->Awake();
        m_awakeDone = true;
    }

    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject->IsActive())
            gameObject->Update(dt);
    }
}

void shak::Scene::Render()
{
    m_drawables.clear();
    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject->IsActive())
            m_drawables.emplace_back(gameObject, nullptr);
    }
    m_renderer->Render(m_drawables);
}

void shak::Scene::HandleInput(const sf::Event& event)
{
    for (auto& gameObject : m_gameObjects)
    {
        gameObject->HandleInput(event);
    }
}
