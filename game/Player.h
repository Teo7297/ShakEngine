#pragma once

#include "Ship.h"

class Player : public Ship
{
public:
    Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<shak::TextureAtlas> atlas);
    ~Player() = default;

    void HandleInput(const sf::Event& event) override;

    void Update(float dt) override;


private:
    std::shared_ptr<ShakEngine> m_engine;

};
