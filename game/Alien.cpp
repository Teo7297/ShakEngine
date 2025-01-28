#include "Alien.h"

Alien::Alien(const std::shared_ptr<shak::TextureAtlas> atlas, const std::shared_ptr<Player> player)
    : Ship(atlas, {}), m_player(player)
{
    m_speed = 200.f;
    m_destinationTimer = 5.f;
    m_currentDestinationTimer = 0.f;
}

void Alien::Update(float dt)
{
    // 1. get random angle and set destination to player position at angle + bbox size;
    // 2. if destination is reached, wait 5 seconds
    // 3. repeat
    m_currentDestinationTimer += dt;
    if (m_currentDestinationTimer >= m_destinationTimer)
    {
        const sf::Angle angle = sf::degrees(std::rand() % 360);
        auto bboxSize = m_player->GetVertexArray()->getBounds().size.x;
        // m_destination = m_player->getPosition() + sf::Vector2f(bboxSize, 0.f).rotatedBy(angle);

        m_currentDestinationTimer = 0.f;
    }

    this->UpdateDirection();
    this->UpdateTextureCoords();

    Ship::Update(dt);
}
