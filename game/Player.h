#pragma once

#include "Ship.h"

class Player : public Ship
{
public:
    Player(std::shared_ptr<shak::TextureAtlas> atlas, std::shared_ptr<sf::Texture> laserTexture, std::shared_ptr<sf::Shader> laserShader, std::shared_ptr<shak::TextureAtlas> deathAnimation);
    ~Player() = default;

    void HandleInput(const sf::Event& event) override;
    void Awake() override;
    void Update(float dt) override;

    float Shoot() override;

private:
    std::shared_ptr<sf::Texture> m_laserTexture;
    std::shared_ptr<sf::Shader> m_laserShader;

    float m_totalDamage = 0.0f;
    float m_dpsTimer = 0.0f;
    float m_dps = 0.0f;

    bool m_shooting = false;
    bool m_targetWasSelected = false;
};
