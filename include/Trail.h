#pragma once
#include "EngineDefines.h"

namespace shak
{
    class Trail
    {
    public:
        enum class TrailType
        {
            Strip,
            Line
        };

        Trail(TrailType type);
        ~Trail() = default;

        void UpdateTrail(float dt, const sf::Vector2f& position);
        void DrawTrail(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

        // Setters
        void SetTTL(float ttl) { m_totalLifeTime = ttl; }
        void SetFade(bool fade) { m_fade = fade; }
        void SetColors(sf::Color start, sf::Color end) { m_startColor = start; m_endColor = end; }
        void SetWidths(float start, float end) { m_startWidth = start; m_endWidth = end; }
        void Clear() { m_trailPoints.clear(); };

    private:
        void DrawLine(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
        void DrawStrip(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    private:
        struct TrailPoint
        {
            sf::Vector2f position;
            float ttl;
            sf::Color color;
        };


        TrailType m_type;
        std::vector<TrailPoint> m_trailPoints;
        float m_totalLifeTime;
        bool m_fade;
        sf::Color m_startColor, m_endColor;
        float m_startWidth, m_endWidth;
    };
}