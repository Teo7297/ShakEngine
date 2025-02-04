#include "Scene.h"

namespace shak
{
    Scene::Scene(std::shared_ptr<shak::Renderer> renderer)
        : m_quadtree({ {0, 0}, {10000, 10000} }), m_renderer(renderer), m_root(std::make_shared<GameObject>())
    {
        m_root->Name = "Root";
    }

    void Scene::AddGameObject(const GameObjectPtr& gameObject)
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

    void Scene::Update(float dt)
    {
        if (!m_awakeDone)
        {
            m_root->Awake();
            m_awakeDone = true;
        }

        m_root->Update(dt);
        m_quadtree.update();
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

    void Scene::TestQuadtree(sf::FloatRect area)
    {
        std::cout << "Area: " << area.position.x << ", " << area.position.y << " - " << area.position.x + area.size.x << ", " << area.position.y + area.size.y << std::endl;
        std::vector<GameObjectPtr> retrieved = m_quadtree.query(area);

        std::cout << "Objects retrieved by query: " << retrieved.size() << std::endl;

        // for (size_t i = 0; i < retrieved.size(); ++i) {
        //     std::cout << " - Object at (" << retrieved[i]->getPosition().x
        //         << ", " << retrieved[i]->getPosition().y << ")\n";
        // }

        // auto all = m_quadtree.findAllIntersections();
        // std::cout << "All intersections: " << all.size() << std::endl;
        // for (size_t i = 0; i < all.size(); ++i)
        // {
        //     std::cout << " - " << all[i].first->Name << " at (" << all[i].first->getPosition().x << ", " << all[i].first->getPosition().y << ") collided with " << all[i].second->Name << " at (" << all[i].second->getPosition().x << ", " << all[i].second->getPosition().y << ")\n";
        // }
    }
}