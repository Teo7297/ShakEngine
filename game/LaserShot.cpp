#include "LaserShot.h"
#include "MathExtensions.h"
#include "ShakEngine.h"

LaserShot::LaserShot(const std::shared_ptr<sf::Texture>& texture, const std::shared_ptr<sf::Shader>& shader)
    : shak::Sprite(texture, shader)
    , m_speed(5000.f)
    , m_direction(0.f, 0.f)
    , m_targetDistance(0.f)
{

}


void LaserShot::Update(float dt)
{
    auto movement = m_speed * m_direction * dt;
    m_targetDistance -= movement.length();
    this->move(movement);

    if (m_targetDistance <= 0.f)
    {
        OnHit();
        this->SetActive(false);
        OnHit.Reset(); // LaserShots are expected to be reused, so we reset the event
        return;
    }

    // Shader uniforms
    m_shader->setUniform("u_time", shak::ShakEngine::GetInstance().GetTime()); // TODO: store as member

    shak::Sprite::Update(dt);
}

void LaserShot::Init(const sf::Vector2f& destination, const sf::Color laserColor, const Size laserWidth, bool isRainbow)
{
    m_shader->setUniform("u_laserColor", sf::Glsl::Vec4(laserColor));

    switch (laserWidth)
    {
    case Size::Small:
        m_shader->setUniform("u_laserWidth", 5.f);
        break;
    case Size::Medium:
        m_shader->setUniform("u_laserWidth", 100.f);
        break;
    case Size::Large:
        m_shader->setUniform("u_laserWidth", 500.f);
        break;
    }

    m_shader->setUniform("u_isRainbow", isRainbow);

    auto diff = destination - this->getPosition();
    m_targetDistance = diff.length();
    m_direction = diff / m_targetDistance; // normalize
    auto angle = m_direction.angle();
    this->setRotation(angle);
}
