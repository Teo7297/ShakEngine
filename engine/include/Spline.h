#pragma once

#include "EngineDefines.h"

namespace shak
{
    class Spline
    {
    public:
        Spline(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3);
        ~Spline();

        void UpdatePoint(int index, const sf::Vector2f& p);
        sf::Vector2f GetPoint(float t) const;

    private:
        sf::Vector2f CalculatePointBezier(float t) const;
        // TODO: Add more types of splines

    private:
        sf::Vector2f m_p0, m_p1, m_p2, m_p3;
    };
}