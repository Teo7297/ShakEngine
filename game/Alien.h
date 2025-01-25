#pragma once

#include "Ship.h"
#include "Player.h"

class Alien : public Ship
{
public:
    Alien(const std::shared_ptr<shak::TextureAtlas> atlas, const std::shared_ptr<Player> player);
    ~Alien() = default;

    virtual void Update(float dt) override;

private:
    std::shared_ptr<Player> m_player;
    float m_destinationTimer, m_currentDestinationTimer;
};