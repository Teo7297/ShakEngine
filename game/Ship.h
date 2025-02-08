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

    virtual void Awake() override;
    virtual void Update(float dt) override;

    void ToggleAimSprite(bool show);

protected:
    int GetTextureByDirection() const;
    void UpdateDirection();
    void UpdateLookDirection();
    void UpdateTextureCoords();
    void SetTarget(const std::shared_ptr<Ship>& target);

    virtual float Shoot() { return 0.f; };

    // CALLBACKS
    virtual void OnLaserHit();

private:
    void SpawnDamageNumber(float damage);
    void DisableAimSprite();
    void PlayDeathAnimation();
    void ResetHealth();

protected:
    std::shared_ptr<shak::TextureAtlas> m_atlas;
    sf::Angle m_angleBetweenTextures;
    int m_atlasTexturesCount;

    sf::Vector2f m_direction, m_destination, m_lookDirection;
    sf::Angle m_lookAngle;
    float m_distanceToDestination;

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
    float m_damage;

    // EFFECTS
    std::shared_ptr<shak::Animation> m_deathAnimation;
    std::shared_ptr<shak::Sprite> m_aimSprite;
};