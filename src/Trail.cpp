#include "Trail.h";
#include "MathExtensions.h"

namespace shak
{

    Trail::Trail(TrailType type)
        : m_type(type)
        , m_totalLifeTime(0.0f)
        , m_fade(false)
        , m_startColor(sf::Color::White)
        , m_endColor(sf::Color::White)
        , m_startWidth(0.0f)
        , m_endWidth(0.0f)
    {
    }

    void Trail::UpdateTrail(float dt, const sf::Vector2f& position)
    {
        // 1. Add a new point for the trail
        m_trailPoints.emplace_back(position, m_totalLifeTime, m_startColor);

        // 2. Update the trail points TTL and apply point changes
        for(auto& point : m_trailPoints)
        {
            point.ttl -= dt;

            const auto progress = point.ttl / m_totalLifeTime;

            if(m_startColor != m_endColor)
                point.color = shak::lerp(m_startColor, m_endColor, progress);

            if(m_fade)
                point.color.a = (std::max(0.f, progress) * 255);
        }

        // 3. Remove expired points
        m_trailPoints.erase(
            std::remove_if(m_trailPoints.begin(), m_trailPoints.end(),
                [](const TrailPoint& p) { return p.ttl <= 0.f; }),
            m_trailPoints.end());
    }

    void Trail::DrawTrail(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(m_trailPoints.size() < 2)
            return;

        if(m_type == TrailType::Strip)
            DrawStrip(target, states);
        else if(m_type == TrailType::Line)
            DrawLine(target, states);
    }

    void Trail::DrawLine(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::VertexArray trail(sf::PrimitiveType::LineStrip);

        for(const auto& point : m_trailPoints) {
            sf::Vertex vertex;
            vertex.position = point.position;
            vertex.color = point.color;
            trail.append(vertex);
        }

        target.draw(trail);
    }

    // If a shader is needed, it must be set to the states before calling this function
    void Trail::DrawStrip(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::VertexArray trail(sf::PrimitiveType::TriangleStrip);

        for(size_t i = 0; i < m_trailPoints.size(); ++i) {
            const auto& point = m_trailPoints[i];

            // Skip invalid points
            if(i == 0 || i == m_trailPoints.size() - 1) continue;

            // Direction vector perpendicular to the segment
            const sf::Vector2f& prevPos = m_trailPoints[i - 1].position;
            const sf::Vector2f& nextPos = m_trailPoints[i + 1].position;
            if(prevPos == nextPos) continue;
            sf::Vector2f direction = nextPos - prevPos;
            sf::Vector2f perpendicular(-direction.y, direction.x);
            perpendicular = perpendicular.normalized();

            // Half-width offset
            sf::Vector2f offset = perpendicular * m_startWidth * 0.5f * point.ttl / m_totalLifeTime; // point.ttl / m_totalLifeTime allows to make it pointy by moving the vertices closer to the center

            // Create vertices for the current point
            sf::Vertex top, bottom;
            top.position = point.position + offset;
            top.color = point.color;

            bottom.position = point.position - offset;
            bottom.color = point.color;

            // Add to the trail
            trail.append(top);
            trail.append(bottom);
        }

        // Render the trail
        target.draw(trail, states);
    }
}
