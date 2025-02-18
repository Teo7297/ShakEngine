#pragma once

#include "components/abilities/Ability.h"
#include "LaserShot.h"
#include "GameObjectPool.h"

class Ship;
class Energy;
class MachineGun : public Ability
{
public:
    MachineGun(AbilitySystem* abilitySystem);
    void Update(float dt) override;

private:
    // Ability Logic
    void ShootLaser();

    // Ability Events
    void SetupCallbacks();
    void RegisterCallbacks();
    std::function<void(const GameObjectPtr& target, const sf::Vector2f& pos)> LookAtTarget;
    std::function<void()> AutoAttackOrStop;
    std::function<void(const GameObjectPtr& target, const sf::Vector2f& pos)> StopAutoAttack;
    std::function<void(const GameObjectPtr& target, const sf::Vector2f& pos)> StoreTarget;
    std::function<void(const GameObjectPtr& target, const sf::Vector2f& pos)> SetIsLaserShootingOnShip;
    std::function<void()> StopIsLaserShootingOnShip;
    std::function<void()> OnLaserHit;
    std::function<void()> DeactivateAbility;


private:
    Ship* m_shipOwner;
    std::shared_ptr<Energy> m_energyComponent;
    GameObjectPtr m_target;
    shak::GameObjectPool<LaserShot> m_laserShotPool;
    std::shared_ptr<sf::Texture> m_laserTexture;
    std::shared_ptr<sf::Shader> m_laserShader;
    float m_projectileTimer, m_projectileCooldown;
    float m_costPerShot;
};