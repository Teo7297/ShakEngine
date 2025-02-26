#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    class TrailRenderer : public GameObject
    {
    public:
        enum class TrailType
        {
            Strip,
            Line
        };

        TrailRenderer(TrailType type);
        ~TrailRenderer();

        void Awake() override;
        void Update(float dt) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

        // Setters
        void SetTTL(float ttl) { m_totalLifeTime = ttl; }
        void SetFade(bool fade) { m_fade = fade; }
        void SetColors(sf::Color start, sf::Color end) { m_startColor = start; m_endColor = end; }
        void SetWidths(float start, float end) { m_startWidth = start; m_endWidth = end; }

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