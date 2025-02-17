#pragma once

#include "ShakEngine.h"
#include "Ship.h"
#include "PlayerObj.h"

class Alien : public shak::GameObject
{
public:
    void Awake() override
    {
        m_player = std::dynamic_pointer_cast<PlayerObj>(m_engine->FindGameObjectByName("Player"))->GetShip();
    }

    void Update(float dt) override
    {
        m_currentDestinationTimer += dt;
        if (m_currentDestinationTimer >= m_destinationTimer)
        {
            const sf::Angle angle = sf::degrees(std::rand() % 360);
            auto bboxSize = m_player->GetVertexArray()->getBounds().size.x;
            m_destination = m_player->getPosition() + sf::Vector2f(bboxSize, 0.f).rotatedBy(angle);

            m_currentDestinationTimer = 0.f;
            m_activeShip->SetDestination(m_destination);
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {}

    // void HandleInput(const sf::Event& event) override;

    void SetShip(const std::shared_ptr<Ship>& ship)
    {
        if (m_activeShip)
        {
            m_engine->Destroy(m_activeShip);
            m_activeShip = nullptr;
        }

        m_activeShip = ship;
        m_activeShip->SetControlledByPlayer(true);
        this->AddChild(m_activeShip);
    }

private:
    std::shared_ptr<Ship> m_activeShip;

    float m_currentDestinationTimer = 0.f;
    float m_destinationTimer = 10.f;
    sf::Vector2f m_destination;
    std::shared_ptr<Ship> m_player;

};