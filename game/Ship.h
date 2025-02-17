#pragma once

#include "ShakEngine.h"
#include "GameObjectPool.h"
#include "DamageNumber.h"
#include "LaserShot.h"
#include "Animation.h"
#include "GameObject.h"
#include "JSON.h"

class Ship : public shak::GameObject
{
public:
    Ship() = delete;
    Ship(const json::JSON& shipData);
    virtual ~Ship() = default;

    // TODO: virtual won't be needed in the future
    virtual void Awake() override;
    virtual void Update(float dt) override;
    void HandleInput(const sf::Event& event);

    void ToggleAimSprite(bool show);

    bool IsControlledByPlayer() const { return m_controlledByPlayer; }
    void SetControlledByPlayer(bool controlled) { m_controlledByPlayer = controlled; }

    void SetDestination(const sf::Vector2f& destination) { m_destination = destination; }
    void SetTarget(const std::shared_ptr<Ship>& target);
    bool GetTargetWasSelected() const { return m_targetWasSelected; }
    void ResetTargetWasSelected() { m_targetWasSelected = false; }

protected:
    int GetTextureByDirection() const;
    void UpdateDirection();
    void UpdateLookDirection();
    void UpdateTextureCoords();

    void UpdatePlayerMovementDestination();
    void UpdateAIMovementDestination();

    virtual float Shoot() { return 0.f; };

    // CALLBACKS
    virtual void OnLaserHit();

private:
    void SpawnDamageNumber(float damage);
    void DisableAimSprite();
    void PlayDeathAnimation();
    void ResetHealth();

protected:
    bool m_controlledByPlayer;

    std::shared_ptr<shak::TextureAtlas> m_atlas;
    sf::Angle m_angleBetweenTextures;
    int m_atlasTexturesCount;

    sf::Vector2f m_direction, m_destination, m_lookDirection;
    sf::Angle m_lookAngle;
    float m_distanceToDestination;

    shak::GameObjectPool<DamageNumber> m_damageNumberPool;
    shak::GameObjectPool<LaserShot> m_laserShotPool;
    std::shared_ptr<sf::Texture> m_laserTexture;
    std::shared_ptr<sf::Shader> m_laserShader;

    std::shared_ptr<Ship> m_target;

    // Child points
    std::vector<GameObjectPtr> m_lasers;
    unsigned int m_laserIndex;

    bool m_lookAtTarget;
    bool m_shooting;
    bool m_targetWasSelected;

    // STATS
    json::JSON m_baseStats;
    float m_speed;
    float m_damage;

    // EFFECTS
    std::shared_ptr<shak::Animation> m_deathAnimation;
    std::shared_ptr<shak::Sprite> m_aimSprite;
};