#pragma once

#include "ShakSprite.h"
#include "ShakEvent.h"

class LaserShot : public shak::Sprite
{
public:

    enum class Size
    {
        Small,
        Medium,
        Large
    };

    LaserShot(const sf::Color laserColor, const Size laserWidth, bool isRainbow, const std::shared_ptr<sf::Texture>& texture, const std::shared_ptr<sf::Shader>& shader = nullptr);
    ~LaserShot() = default;

    void Update(float dt) override;

    void Init(const sf::Vector2f& destination);

public:
    shak::Event<> OnHit;

private:
    float m_speed;
    sf::Vector2f m_direction;
    float m_targetDistance;
};