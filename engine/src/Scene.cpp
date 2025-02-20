#include "Scene.h"

namespace shak
{
    Scene::Scene(std::shared_ptr<shak::Renderer> renderer)
        : m_quadtree({ {0, 0}, {10000, 10000} })
        , m_root(std::make_shared<GameObject>())
        , m_drawables()
        , m_renderer(renderer)
        , m_awakeDone(false)
    {
        m_root->Name = "Root";
    }

    void Scene::AddGameObject(const GameObjectPtr gameObject)
    {
        m_root->AddChild(gameObject);
    }

    void Scene::AddGameObjectToQuadtree(const GameObjectPtr& gameObject)
    {
        m_quadtree.add(gameObject);
    }

    void Scene::RemoveGameObject(int id)
    {
        bool result = m_root->RemoveChildRecursive(id);
        if (!result)
            std::cerr << "[Scene - Destroy] GameObject with id " << id << " not found" << std::endl;
    }

    GameObjectPtr Scene::FindGameObject(const std::string& name) const
    {
        return m_root->FindChildRecursive(name);
    }

    GameObjectPtr Scene::FindGameObject(int id) const
    {
        return m_root->FindChildRecursive(id);
    }

    void Scene::ForwardAwake()
    {
        m_root->ForwardAwake();
    }

    void Scene::Update(float dt)
    {
        if (!m_awakeDone)
        {
            m_root->Awake();
            m_awakeDone = true;
        }

        m_root->Update(dt);
        m_quadtree.update();

        m_root->LateUpdate(dt);
    }

    void Scene::Render()
    {
        m_drawables.clear();

        // traverse the tree and add all drawables to the list
        m_root->GetDrawables(m_drawables);

        std::ranges::sort(m_drawables, [](const GameObjectPtr& a, const GameObjectPtr& b)
            {
                return a->GetZIndex() < b->GetZIndex();
            });

        m_renderer->Render(m_drawables);
    }

    void Scene::Cleanup()
    {
        m_root->Cleanup();
    }

    void Scene::HandleInput(const sf::Event& event)
    {
        m_root->HandleInput(event);
    }

    void Scene::CheckCollisions()
    {
        auto collisions = m_quadtree.findAllIntersections();

        for (const auto& [a, b] : collisions)
        {
            a->OnCollision(b);
            b->OnCollision(a);
        }
    }
}