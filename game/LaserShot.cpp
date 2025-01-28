#include "LaserShot.h"

LaserShot::LaserShot(const sf::Color laserColor, const Size laserWidth, bool isRainbow, const std::shared_ptr<sf::Texture>& texture, const std::shared_ptr<sf::Shader>& shader)
    : shak::Sprite(texture, shader)
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
}


void LaserShot::Update(float dt)
{
    auto movement = m_direction * m_speed * dt;
    this->move(movement);

    m_targetDistance -= movement.length();
    if (m_targetDistance <= 0.f)
    {
        this->SetActive(false);
    }
    shak::Sprite::Update(dt);
}

void LaserShot::Init(const sf::Vector2f& target, sf::Angle angle)
{
    m_targetDistance = (target - this->getPosition()).length();
    m_direction = sf::Vector2f(1.f, 0.f).rotatedBy(angle);
    this->setRotation(angle);
}
