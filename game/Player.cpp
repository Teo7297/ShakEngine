#include "Player.h"

#include "LaserShot.h"

Player::Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<shak::TextureAtlas> atlas, std::shared_ptr<sf::Texture> laserTexture, std::shared_ptr<sf::Shader> laserShader)
    : Ship(atlas), m_engine(engine), m_laserTexture(laserTexture), m_laserShader(laserShader)
{
}

void Player::HandleInput(const sf::Event& event)
{
    if (auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Space)
        {
            Shoot();
        }
    }

    Ship::HandleInput(event);
}

void Player::Awake()
{
    m_target = std::dynamic_pointer_cast<Ship>(m_engine->FindGameObjectByName("Alien"));
    Ship::Awake();
}

void Player::Update(float dt)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        // Move if click outside of ship
        auto mousePos = m_engine->GetMouseWorldPos();
        bool updateDestination = !this->IsPointInside(mousePos);

        // Click valid only if inside window
        auto windowSize = m_engine->GetWindowSize();
        auto mousePosPixel = m_engine->GetMousePixelPos();
        updateDestination &= mousePosPixel.x >= 0 && mousePosPixel.x <= windowSize.x;
        updateDestination &= mousePosPixel.y >= 0 && mousePosPixel.y <= windowSize.y;

        if (updateDestination)
        {
            m_destination = mousePos;
            this->UpdateDirection();
        }
    }
    this->UpdateTextureCoords();

    static float time = 0.f;
    time += dt;
    m_laserShader->setUniform("u_time", time);

    Ship::Update(dt);
}

float Player::Shoot()
{
    if (!m_target)
        return 0.f;

    m_lookAtTarget = true;
    auto direction = m_target->getPosition() - this->getPosition();
    direction = direction.normalized();
    sf::Angle laserAngle = direction.angle();
    auto shot = std::make_shared<LaserShot>(laserAngle, sf::Color::Blue, LaserShot::Size::Large, false, m_laserTexture, m_laserShader);
    shot->setPosition(this->getPosition());
    this->AddChild(shot);

    return m_target->TakeDamage(m_damage);
}
