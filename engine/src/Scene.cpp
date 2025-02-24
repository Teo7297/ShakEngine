#include "Scene.h"

#include "UIManager.h"
#include "UIElement.h"

#include "shapes/Line.h"
#include "shapes/Square.h"
#include "shapes/Circle.h"

namespace shak
{
    Scene::Scene(std::shared_ptr<shak::Renderer> renderer)
        : m_quadtree({ {0, 0}, {10000, 10000} })
        , m_root(std::make_shared<GameObject>())
        , m_drawables()
        , m_renderer(renderer)
        , m_awakeDone(false)
        , m_ui(std::make_shared<shak::UIManager>())
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

    void Scene::AddUIElement(const std::string& name, const std::shared_ptr<UIElement>& element)
    {
        m_ui->AddUIElement(name, element);
    }

    void Scene::RemoveUIElement(const std::string& name)
    {
        m_ui->RemoveUIElement(name);
    }

    std::shared_ptr<UIElement> Scene::GetUIElement(const std::string& name)
    {
        return m_ui->GetUIElement(name);
    }

    void Scene::SelectActiveUI(const std::string& name)
    {
        m_ui->SelectActiveUI(name);
    }

    void Scene::DeselectActiveUI()
    {
        m_ui->DeselectActiveUI();
    }

    std::shared_ptr<UIElement> Scene::GetActiveUI()
    {
        return m_ui->GetActiveUI();
    }

    std::optional<sf::Vector2f> getRotatedRectIntersectionPoint(
        const sf::Vector2f& origin, const sf::Vector2f& direction,
        float maxDistance,
        const sf::Vector2f& rectCenter, const sf::Vector2f& rectSize, const sf::Angle& rotation)
    {
        // Transform the origin and direction into the rectangle's local coordinate system:
        // translate by -rectCenter then rotate by -rotation.
        sf::Vector2f localOrigin = origin - rectCenter;
        localOrigin = localOrigin.rotatedBy(-rotation);
        auto localDirection = direction.rotatedBy(-rotation);
        // Construct an axis-aligned rectangle in local space.
        sf::FloatRect localRect;
        localRect.position = { -rectSize.x / 2.f, -rectSize.y / 2.f };
        localRect.size = rectSize;

        // Compute rectangle boundaries using SFML 3's members.
        float leftEdge = localRect.position.x;
        float rightEdge = localRect.position.x + localRect.size.x;
        float topEdge = localRect.position.y;
        float bottomEdge = localRect.position.y + localRect.size.y;

        float t0 = 0.f;
        float t1 = maxDistance;

        // X-axis test
        if (std::abs(localDirection.x) < 1e-6f)
        {
            if (localOrigin.x < leftEdge || localOrigin.x > rightEdge)
                return std::nullopt;
        }
        else
        {
            float invDx = 1.f / localDirection.x;
            float tNear = (leftEdge - localOrigin.x) * invDx;
            float tFar = (rightEdge - localOrigin.x) * invDx;
            if (tNear > tFar)
                std::swap(tNear, tFar);
            t0 = std::max(t0, tNear);
            t1 = std::min(t1, tFar);
            if (t0 > t1)
                return std::nullopt;
        }

        // Y-axis test
        if (std::abs(localDirection.y) < 1e-6f)
        {
            if (localOrigin.y < topEdge || localOrigin.y > bottomEdge)
                return std::nullopt;
        }
        else
        {
            float invDy = 1.f / localDirection.y;
            float tNear = (topEdge - localOrigin.y) * invDy;
            float tFar = (bottomEdge - localOrigin.y) * invDy;
            if (tNear > tFar)
                std::swap(tNear, tFar);
            t0 = std::max(t0, tNear);
            t1 = std::min(t1, tFar);
            if (t0 > t1)
                return std::nullopt;
        }

        if (t0 <= maxDistance)
        {
            // Intersection in local coordinates.
            sf::Vector2f localIntersection = localOrigin + localDirection * t0;
            // Transform back: rotate by rotation, then translate by rectCenter.
            sf::Vector2f worldIntersection = localIntersection.rotatedBy(rotation) + rectCenter;
            return worldIntersection;
        }
        return std::nullopt;
    }

    void Scene::Raycast(const sf::Vector2f& origin, const sf::Vector2f& direction, float maxDistance, std::vector<RaycastHit>& outHits, bool drawDebug)
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

            if (auto opt = getRotatedRectIntersectionPoint(origin, direction, maxDistance, bounds.getCenter(), bounds.size, obj->getRotation()))
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

        // Sort by distance
        std::ranges::sort(outHits, [](const RaycastHit& a, const RaycastHit& b)
            {
                return a.distance < b.distance;
            });

        if (drawDebug)
        {
            sf::Color lineColor = outHits.empty() ? sf::Color::Red : sf::Color::Green;

            auto hitLine = std::make_shared<Line>(
                origin,
                origin + direction * maxDistance,
                lineColor
            );
            hitLine->SetZIndex(1500);
            m_root->AddChild(hitLine);

            for (const auto& hitData : outHits)
            {
                auto hitSquare = std::make_shared<Square>(sf::FloatRect{ hitData.hitPoint, {10.f, 10.f} }, sf::Color::Green);
                m_root->AddChild(hitSquare);
                hitSquare->SetZIndex(1500);
            }
        }
    }

    std::optional<sf::Vector2f> getRotatedRectCircleIntersectionPoint(
        const sf::Vector2f& circleCenter, float radius,
        const sf::Vector2f& rectCenter, const sf::Vector2f& rectSize, const sf::Angle& rotation)
    {
        // Transform circle center into rectangle's local coordinate system.
        sf::Vector2f localCircleCenter = circleCenter - rectCenter;
        localCircleCenter = localCircleCenter.rotatedBy(-rotation);

        // The rectangle is axis-aligned in local space.
        float halfWidth = rectSize.x / 2.f;
        float halfHeight = rectSize.y / 2.f;

        // Find the closest point on the rectangle to the circle center by clamping.
        float clampedX = std::max(-halfWidth, std::min(localCircleCenter.x, halfWidth));
        float clampedY = std::max(-halfHeight, std::min(localCircleCenter.y, halfHeight));
        sf::Vector2f closestLocalPoint(clampedX, clampedY);

        // Compute the distance between the circle's center and this clamped point.
        sf::Vector2f delta = localCircleCenter - closestLocalPoint;
        float distanceSquared = delta.x * delta.x + delta.y * delta.y;

        if (distanceSquared <= radius * radius)
        {
            // There's an intersection. Transform the contact point back to world space.
            sf::Vector2f worldIntersection = closestLocalPoint.rotatedBy(rotation) + rectCenter;
            return worldIntersection;
        }

        return std::nullopt;
    }

    void Scene::Circlecast(const sf::Vector2f& center, float radius, std::vector<RaycastHit>& outHits, bool drawDebug)
    {
        // Make sure this is empty
        outHits.clear();
        outHits.shrink_to_fit();

        auto topLeftArea = center - sf::Vector2f(radius, radius);
        sf::FloatRect searchArea(topLeftArea, sf::Vector2f(2 * radius, 2 * radius));
        auto candidates = m_quadtree.query(searchArea);

        for (const auto& obj : candidates)
        {
            if (obj->Name == "Player")continue;
            const sf::FloatRect bounds({ obj->getPosition() - obj->getOrigin(), obj->GetVertexArray()->getBounds().size });

            if (auto opt = getRotatedRectCircleIntersectionPoint(center, radius, bounds.getCenter(), bounds.size, obj->getRotation()))
            {
                auto hitPt = opt.value();
                RaycastHit hitData =
                {
                    .hitObject = obj,
                    .hitPoint = hitPt,
                    .normal = {0, 0}, // TODO: implement
                    .distance = (hitPt - center).length()
                };
                outHits.push_back(hitData);
            }
        }

        // Sort by distance
        std::ranges::sort(outHits, [](const RaycastHit& a, const RaycastHit& b)
            {
                return a.distance < b.distance;
            });

        if (drawDebug)
        {
            if (outHits.empty())
            {
                auto hitCircle = std::make_shared<Circle>(
                    center,
                    radius,
                    sf::Color::Red,
                    Circle::Type::Outlined
                );
                m_root->AddChild(hitCircle);
            }

            for (const auto& hitData : outHits)
            {
                auto hitCircle = std::make_shared<Circle>(
                    center,
                    hitData.distance,
                    sf::Color::Green,
                    Circle::Type::Outlined
                );
                m_root->AddChild(hitCircle);

                auto hitSquare = std::make_shared<Square>(sf::FloatRect{ hitData.hitPoint, {10.f, 10.f} }, sf::Color::Green);
                m_root->AddChild(hitSquare);
            }
        }
    }

    void Scene::ForwardAwake()
    {
        m_root->ForwardAwake();
    }

    void Scene::TryInitActiveUI()
    {
        const auto& activeUI = m_ui->GetActiveUI();
        if (activeUI && !activeUI->InitDone)
        {
            activeUI->Init();
            activeUI->InitDone = true; //? I prefer doing it here and avoid UIElement::Init() in every UI element instance
        }
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

    void Scene::DrawUI()
    {
        m_ui->DrawUI();
    }
}