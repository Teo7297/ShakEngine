#pragma once
#include "EngineDefines.h"

namespace shak
{
    inline sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
    {
        return (1 - t) * a + t * b;
    }

    inline sf::Vector2f slerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
    {
        float dot = a.x * b.x + a.y * b.y;
        dot = std::clamp(dot, -1.f, 1.f);

        // If vectors are nearly opposite, fallback to linear interpolation to avoid numerical issues.
        if (std::abs(dot + 1.f) < 1e-6)
        {
            return lerp(a, b, t);
        }

        float theta = std::acos(dot) * t;
        sf::Vector2f relative = b - a * dot;
        float len = std::sqrt(relative.x * relative.x + relative.y * relative.y);

        // Avoid division by zero (if len is extremely small, fallback gracefully)
        if (len < 1e-6)
        {
            return a;
        }

        relative = relative / len;
        return a * std::cos(theta) + relative * std::sin(theta);
    }
}