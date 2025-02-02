#include "Scene.h"

namespace shak
{
    Scene::Scene(std::shared_ptr<shak::Renderer> renderer)
        : m_renderer(renderer), m_root(std::make_shared<GameObject>())
    {
        m_root->Name = "Root";
    }

    void Scene::AddGameObject(GameObjectPtr gameObject)
    {
        m_root->AddChild(gameObject);
    }

    void Scene::RemoveGameObject(int id)
    {
        bool result = m_root->RemoveChildRecursive(id);
        if (!result)
            std::cerr << "[Scene - Destroy] GameObject with id " << id << " not found" << std::endl;
    }

    GameObjectPtr Scene::FindGameObject(std::string name) const
    {
        return m_root->FindChildRecursive(name);
    }

    GameObjectPtr Scene::FindGameObject(int id) const
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

        // traverse the tree and add all drawables to the list
        m_root->GetDrawables(m_drawables);

        std::sort(m_drawables.begin(), m_drawables.end(), [](const GameObjectPtr& a, const GameObjectPtr& b)
            {
                return a->GetZIndex() < b->GetZIndex();
            });

        m_renderer->Render(m_drawables);
    }

    void Scene::HandleInput(const sf::Event& event)
    {
        m_root->HandleInput(event);
    }
}