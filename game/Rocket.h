#pragma once

#include "ShakSprite.h"
#include "Spline.h"
#include "Ship.h"
#include "ShakEvent.h"

namespace shak
{
    class ParticleSystem;
}

class Rocket : public shak::Sprite
{
public:
    Rocket(const std::shared_ptr<sf::Texture>& texture, const std::shared_ptr<sf::Shader>& shader);

    void Init(const sf::Vector2f& start, const std::shared_ptr<Ship>& target);

    void Awake() override;
    void Update(float dt) override;

public:
    shak::Event<const std::shared_ptr<Ship>&> OnHit;

private:
    sf::Vector2f m_start, m_destination;
    std::unique_ptr<shak::Spline> m_spline;
    sf::Vector2f m_previousPos;
    float m_t;

    std::shared_ptr<shak::Animation> m_explosion;
    std::shared_ptr<shak::ParticleSystem> m_ps;

    std::shared_ptr<Ship> m_target;
};