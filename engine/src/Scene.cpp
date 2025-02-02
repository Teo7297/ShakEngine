#include "Scene.h"

namespace shak
{
    Scene::Scene(std::shared_ptr<shak::Renderer> renderer)
        : m_renderer(renderer), m_root(std::make_shared<GameObject>())
    {
        m_root->Name = "Root";
    }

    void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
    {
        m_root->AddChild(gameObject);
    }

    void Scene::RemoveGameObject(int id)
    {
        bool result = m_root->RemoveChildRecursive(id);
        if (!result)
            std::cerr << "[Scene - Destroy] GameObject with id " << id << " not found" << std::endl;
    }

    std::shared_ptr<GameObject> Scene::FindGameObject(std::string name) const
    {
        return m_root->FindChildRecursive(name);
    }

    std::shared_ptr<GameObject> Scene::FindGameObject(int id) const
    {
        return m_root->FindChildRecursive(id);
    }    

    void Scene::Update(float dt)
    {
        if (!m_awakeDone)
        {
            m_root->Awake();
            m_awakeDone = true;
        }

        m_root->Update(dt);
    }

    void Scene::Render()
    {
        m_drawables.clear();

        m_drawables.emplace_back(m_root, nullptr);

        m_renderer->Render(m_drawables);
    }

    void Scene::HandleInput(const sf::Event& event)
    {
        m_root->HandleInput(event);
    }
}