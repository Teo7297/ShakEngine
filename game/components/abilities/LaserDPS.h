#pragma once

#include "components/abilities/Ability.h"
#include "LaserShot.h"
#include "GameObjectPool.h"

class Energy;
class Ship;
class LaserDPS : public Ability
{
public:
    LaserDPS(AbilitySystem* abilitySystem);

    void Update(float dt) override;

private:
    // Ability Logic
    void ShootLaser();

    // Ability Events
    void SetupCallbacks();
    void RegisterCallbacks();
    std::function<void(const GameObjectPtr& target, const sf::Vector2f& pos)> ShootLaserCB;
    std::function<void(const GameObjectPtr& target, const sf::Vector2f& pos)> LookAtTarget;
    std::function<void()> StopLookAtTarget;
    std::function<void()> Reactivate;
    std::function<void(const GameObjectPtr&)> AutoAttackStart;
    std::function<void()> AutoAttackStop;

private:
    Ship* m_shipOwner;
    bool m_shooting;
    GameObjectPtr m_target;
    shak::GameObjectPool<LaserShot> m_laserShotPool;
    std::shared_ptr<sf::Texture> m_laserTexture;
    std::shared_ptr<sf::Shader> m_laserShader;
    std::shared_ptr<Energy> m_energyComponent;
    float m_energyPerShot;
};