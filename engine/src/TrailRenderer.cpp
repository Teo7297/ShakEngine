#include "TrailRenderer.h"

shak::TrailRenderer::TrailRenderer(TrailType type)
    : GameObject()
    , m_type(type)
    , m_totalLifeTime(0.0f)
    , m_fade(false)
{
}

shak::TrailRenderer::~TrailRenderer()
{
}

void shak::TrailRenderer::Awake()
{
    this->GetParent()->OnDisable += [this]() { m_trailPoints.clear();};
}

void shak::TrailRenderer::Update(float dt)
{
    // 1. Add a new point for the trail
    m_trailPoints.emplace_back(this->getPosition(), m_totalLifeTime, m_startColor);

    // 2. Update the trail points TTL and apply point changes
    for (auto& point : m_trailPoints)
    {
        point.ttl -= dt;

        const auto progress = point.ttl / m_totalLifeTime;

        if (m_fade)
            point.color.a = (std::max(0.f, progress) * 255);
    }

    // 3. Remove expired points
    m_trailPoints.erase(
        std::remove_if(m_trailPoints.begin(), m_trailPoints.end(),
            [](const TrailPoint& p) { return p.ttl <= 0.f; }),
        m_trailPoints.end());
}

void shak::TrailRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_trailPoints.size() < 2)
        return;

    if (m_type == TrailType::Strip)
        DrawStrip(target, states);
    else if (m_type == TrailType::Line)
        DrawLine(target, states);
}

void shak::TrailRenderer::DrawLine(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::VertexArray trail(sf::PrimitiveType::LineStrip);

    for (const auto& point : m_trailPoints) {
        sf::Vertex vertex;
        vertex.position = point.position;
        vertex.color = point.color;
        trail.append(vertex);
    }

    target.draw(trail);
}

void shak::TrailRenderer::DrawStrip(sf::RenderTarget& target, sf::RenderStates states) const
{
}
