#pragma once

#include "Ship.h"

class Player : public Ship
{
public:
    Player(const json::JSON& shipData);
    ~Player() = default;

    void HandleInput(const sf::Event& event) override;
    void Awake() override;
    void Update(float dt) override;

    float Shoot() override;

private:
    float m_totalDamage = 0.0f;
    float m_dpsTimer = 0.0f;
    float m_dps = 0.0f;

    bool m_shooting = false;
    bool m_targetWasSelected = false;
};
