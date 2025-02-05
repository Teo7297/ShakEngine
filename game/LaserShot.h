#pragma once

#include "Sprite.h"
#include "ShakEvent.h"


class LaserShot : public shak::Sprite
{
public:

    struct HitInfo
    {
        float damage;
        bool killed;
    };

    enum class Size
    {
        Small,
        Medium,
        Large
    };

    LaserShot(const sf::Color laserColor, const Size laserWidth, bool isRainbow, const std::shared_ptr<sf::Texture>& texture, const std::shared_ptr<sf::Shader>& shader = nullptr);
    ~LaserShot() = default;

    void Update(float dt) override;

    void Init(const sf::Vector2f& target, sf::Angle angle);

public:
    shak::Event<HitInfo> OnHit;

private:
    float m_speed = 5000.f;
    sf::Vector2f m_direction = { 0.f, -1.f };
    float m_targetDistance = 0.f;
};