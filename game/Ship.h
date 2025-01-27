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
    virtual void Awake() override;
    virtual void Update(float dt) override;

    virtual float TakeDamage(float damage);

protected:
    int GetTextureByDirection() const;
    void UpdateDirection();
    void UpdateTextureCoords();
    void SetTarget(std::shared_ptr<Ship> target);

    virtual float Shoot() { return -1.f; }

protected:
    std::shared_ptr<shak::TextureAtlas> m_atlas;
    int m_atlasTexturesCount;

    sf::Vector2f m_direction = { 1.f, 0.f };
    sf::Vector2f m_destination;

    shak::GameObjectPool<DamageNumber> m_damageNumberPool;

    std::shared_ptr<Ship> m_target;

    // STATS
    bool m_lookAtTarget = false;
    float m_speed = 1000.f;
    float m_hp = 100000.f;
    float m_maxHp = 100000.f;
    float m_damage = 1000.f;
    float m_shield = 100000.f;
};