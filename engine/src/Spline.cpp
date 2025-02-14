#include "Spline.h"

namespace shak
{
    Spline::Spline(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3)
        : m_p0(p0)
        , m_p1(p1)
        , m_p2(p2)
        , m_p3(p3)
    {
    }

    Spline::~Spline()
    {
    }

    sf::Vector2f Spline::GetPoint(float t) const
    {
        return CalculatePointBezier(t);
    }

    sf::Vector2f Spline::CalculatePointBezier(float t) const
    {
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        sf::Vector2f p = m_p0 * uuu;
        p = p + m_p1 * (3 * uu * t);
        p = p + m_p2 * (3 * u * tt);
        p = p + m_p3 * ttt;

        return p;
    }


}