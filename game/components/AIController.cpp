#include "AIController.h"
#include "Ship.h"

void AIController::Awake()
{
    m_engine = &shak::ShakEngine::GetInstance();
    m_player = std::dynamic_pointer_cast<Ship>(m_engine->FindGameObjectByName("Player"));
    m_ownerShip = (Ship*)m_owner;
}

void AIController::Update(float dt)
{
    m_currentDestinationTimer += dt;
    if (m_currentDestinationTimer >= m_destinationTimer)
    {
        const sf::Angle angle = sf::degrees(std::rand() % 360);
        auto bboxSize = m_player->GetVertexArray()->getBounds().size.x;
        m_destination = m_player->getPosition() + sf::Vector2f(bboxSize, 0.f).rotatedBy(angle);

        m_currentDestinationTimer = 0.f;
        m_ownerShip->SetDestination(m_destination);
    }
}
