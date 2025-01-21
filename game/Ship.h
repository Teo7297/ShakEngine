#pragma once

#include "ShakEngine.h"
#include "GameObjectPool.h"
#include "DamageNumber.h"


class Ship : public shak::GameObject
{
public:
    Ship(const std::shared_ptr<shak::TextureAtlas> atlas);
    virtual ~Ship() = default;

    virtual void HandleInput(const sf::Event& event) override;

    virtual void Update(float dt) override;

protected:
    int GetTextureByDirection() const;
    void UpdateDirection();
    void UpdateTextureCoords();

protected:
    std::shared_ptr<shak::TextureAtlas> m_atlas;
    int m_atlasTexturesCount;

    float m_speed = 1000.f;
    sf::Vector2f m_direction;
    sf::Vector2f m_destination;

    shak::GameObjectPool<DamageNumber> m_damageNumberPool;
};