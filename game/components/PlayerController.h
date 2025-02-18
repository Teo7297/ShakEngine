#pragma once

#include "ShakEngine.h"
#include "Component.h"

class PlayerController : public shak::Component
{
public:
    PlayerController(shak::GameObject* owner)
        : shak::Component(owner)
    {
    }

    ~PlayerController() override = default;

    void Awake() override;
    void Update(float dt) override;
    void HandleInput(const sf::Event& event) override;

private:
    shak::ShakEngine* m_engine;
    class Ship* m_ownerShip;
    std::shared_ptr<class AbilitySystem> m_abilitySystem;
};
