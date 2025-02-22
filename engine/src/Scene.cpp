#include "Scene.h"

#include "shapes/Line.h"
#include "shapes/Square.h"

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

    // Returns true if the line segment (origin -> origin + direction * maxDistance)
    // intersects the axis-aligned rectangle 'rect'
    std::optional<sf::Vector2f> getLineRectIntersectionPoint(const sf::Vector2f& origin,
        const sf::Vector2f& direction,
        float maxDistance,
        const sf::FloatRect& rect)
    {
        // Calculate rectangle boundaries
        float leftEdge = rect.position.x;
        float rightEdge = rect.position.x + rect.size.x;
        float topEdge = rect.position.y;
        float bottomEdge = rect.position.y + rect.size.y;

        // Initialize the parametric interval [t0, t1]
        float t0 = 0.f;
        float t1 = maxDistance;

        // X-axis test
        if (std::abs(direction.x) < 1e-6f)
        {
            if (origin.x < leftEdge || origin.x > rightEdge)
                return std::nullopt;
        }
        else
        {
            float invDx = 1.f / direction.x;
            float tNear = (leftEdge - origin.x) * invDx;
            float tFar = (rightEdge - origin.x) * invDx;
            if (tNear > tFar)
                std::swap(tNear, tFar);
            t0 = std::max(t0, tNear);
            t1 = std::min(t1, tFar);
            if (t0 > t1)
                return std::nullopt;
        }

        // Y-axis test
        if (std::abs(direction.y) < 1e-6f)
        {
            if (origin.y < topEdge || origin.y > bottomEdge)
                return std::nullopt;
        }
        else
        {
            float invDy = 1.f / direction.y;
            float tNear = (topEdge - origin.y) * invDy;
            float tFar = (bottomEdge - origin.y) * invDy;
            if (tNear > tFar)
                std::swap(tNear, tFar);
            t0 = std::max(t0, tNear);
            t1 = std::min(t1, tFar);
            if (t0 > t1)
                return std::nullopt;
        }

        // If t0 is within the valid range, compute the intersection point.
        if (t0 <= maxDistance)
            return origin + direction * t0;

        return std::nullopt;
    }

    void Scene::RaycastOne(const sf::Vector2f& origin, const sf::Vector2f& direction, float maxDistance, RaycastHit& outHit, bool drawDebug)
    {
        sf::FloatRect searchArea(origin, direction * maxDistance);
        auto candidates = m_quadtree.query(searchArea);

        bool somethingHit = false;
        for (const auto& obj : candidates)
        {
            if (obj->Name == "Player")continue;
            const sf::FloatRect bounds({ obj->getPosition() - obj->getOrigin(), obj->GetVertexArray()->getBounds().size });

            if (auto opt = getLineRectIntersectionPoint(origin, direction, maxDistance, bounds))
            {
                auto hitPt = opt.value();
                outHit =
                {
                    .hitObject = obj,
                    .hitPoint = hitPt,
                    .normal = {0, 0}, // TODO: implement
                    .distance = (hitPt - origin).length()
                };
                somethingHit = true;
                break;
            }
        }

        if (drawDebug)
        {
            auto hitLine = std::make_shared<Line>(
                origin,
                somethingHit ? outHit.hitPoint : origin + direction * maxDistance,
                somethingHit ? sf::Color::Green : sf::Color::Red
            );
            m_root->AddChild(hitLine);
            if (somethingHit)
            {
                auto hitSquare = std::make_shared<Square>(sf::FloatRect{ outHit.hitPoint, {10.f, 10.f} }, sf::Color::Green);
                m_root->AddChild(hitSquare);
            }
        }
    }

    void Scene::RaycastAll(const sf::Vector2f& origin, const sf::Vector2f& direction, float maxDistance, std::vector<RaycastHit>& outHits, bool drawDebug)
    {
        // Make sure this is empty
        outHits.clear();
        outHits.shrink_to_fit();

        sf::FloatRect searchArea(origin, direction * maxDistance);
        auto candidates = m_quadtree.query(searchArea);

        for (const auto& obj : candidates)
        {
            if (obj->Name == "Player")continue;
            const sf::FloatRect bounds({ obj->getPosition() - obj->getOrigin(), obj->GetVertexArray()->getBounds().size });

            if (auto opt = getLineRectIntersectionPoint(origin, direction, maxDistance, bounds))
            {
                auto hitPt = opt.value();
                RaycastHit hitData =
                {
                    .hitObject = obj,
                    .hitPoint = hitPt,
                    .normal = {0, 0}, // TODO: implement
                    .distance = (hitPt - origin).length()
                };
                outHits.push_back(hitData);
            }
        }

        if (drawDebug)
        {
            if (outHits.empty())
            {
                auto hitLine = std::make_shared<Line>(
                    origin,
                    origin + direction * maxDistance,
                    sf::Color::Red
                );
                m_root->AddChild(hitLine);
            }

            for (const auto& hitData : outHits)
            {
                auto hitLine = std::make_shared<Line>(
                    origin,
                    hitData.hitPoint,
                    sf::Color::Green
                );
                m_root->AddChild(hitLine);

                auto hitSquare = std::make_shared<Square>(sf::FloatRect{ hitData.hitPoint, {10.f, 10.f} }, sf::Color::Green);
                m_root->AddChild(hitSquare);
            }
        }
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