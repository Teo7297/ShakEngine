#pragma once

#include "ShakEngine.h"
#include "Component.h"

class Ship;
class AIController : public shak::Component
{
public:
    AIController(shak::GameObject* owner)
        : shak::Component(owner)
    {
    }

    ~AIController() override = default;

    void Awake() override;
    void Update(float dt) override;

private:
    shak::ShakEngine* m_engine;
    Ship* m_ownerShip;
    std::shared_ptr<Ship> m_player;

    // Movement variables
    float m_currentDestinationTimer = 0.f;
    float m_destinationTimer = 10.f;
    sf::Vector2f m_destination;
};