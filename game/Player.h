#pragma once

#include "Ship.h"

class Player : public Ship
{
public:
    Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<shak::TextureAtlas> atlas, std::shared_ptr<sf::Texture> laserTexture, std::shared_ptr<sf::Shader> laserShader);
    ~Player() = default;

    void HandleInput(const sf::Event& event) override;
    void Awake() override;
    void Update(float dt) override;

    float Shoot() override;

private:
    std::shared_ptr<ShakEngine> m_engine;
    std::shared_ptr<sf::Texture> m_laserTexture;
    std::shared_ptr<sf::Shader> m_laserShader;
};
