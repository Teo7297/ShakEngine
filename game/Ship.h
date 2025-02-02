#pragma once

#include "ShakEngine.h"
#include "GameObjectPool.h"
#include "DamageNumber.h"
#include "LaserShot.h"
#include "Animation.h"
#include "GameObject.h"

class Ship : public shak::GameObject
{
public:
    Ship() = delete;
    Ship(const std::shared_ptr<shak::TextureAtlas> atlas, const std::vector<sf::Vector2f> lasersOffsets, const std::shared_ptr<shak::TextureAtlas> deathAnimation);
    virtual ~Ship() = default;

    virtual void HandleInput(const sf::Event& event) override;
    virtual void Awake() override;
    virtual void Update(float dt) override;

    virtual LaserShot::HitInfo TakeDamage(float damage);

    void ToggleAimSprite(bool show);

protected:
    int GetTextureByDirection() const;
    void UpdateDirection();
    void UpdateLookDirection();
    void UpdateTextureCoords();
    void SetTarget(std::shared_ptr<Ship> target);

    virtual float Shoot() { return 0.f; };

    // CALLBACKS
    virtual LaserShot::HitInfo OnLaserHit();

protected:
    std::shared_ptr<shak::TextureAtlas> m_atlas;
    int m_atlasTexturesCount;

    sf::Vector2f m_direction, m_lookDirection, m_destination;

    shak::GameObjectPool<DamageNumber> m_damageNumberPool;
    shak::GameObjectPool<LaserShot> m_laserShotPool;

    std::shared_ptr<Ship> m_target;

    // Child points
    std::vector<GameObjectPtr> m_lasers;
    std::vector<sf::Vector2f> m_laserOffsets;
    unsigned int m_laserIndex;

    // STATS
    bool m_lookAtTarget = false;
    float m_speed;
    float m_hp;
    float m_maxHp;
    float m_damage;
    float m_shield;

    // EFFECTS
    std::shared_ptr<shak::Animation> m_deathAnimation;
    std::shared_ptr<shak::Sprite> m_aimSprite;
};