#pragma once

#include "ShakEngine.h"
#include "GameObjectPool.h"

#include "DamageNumber.h"

class Player : public shak::GameObject
{
public:
    Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<sf::VertexArray> va, std::shared_ptr<shak::TextureAtlas> atlas);
    ~Player() = default;

    void HandleInput(const sf::Event& event) override;

    void Update(float dt) override;

private:
    int GetTextureByDirection() const;

private:
    float m_speed = 1000.f;
    sf::Vector2f m_direction;
    sf::Vector2f m_destination;
    std::shared_ptr<ShakEngine> m_engine;

    sf::Vector2f m_bbSize;

    std::shared_ptr<shak::TextureAtlas> m_atlas;
    int m_atlasTexturesCount;

    shak::GameObjectPool<DamageNumber> m_damageNumberPool;
};
