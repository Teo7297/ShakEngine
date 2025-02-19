#pragma once

#include "components/abilities/Ability.h"

class LifeDrain : public Ability
{
public:
    LifeDrain(AbilitySystem* abilitySystem);
    void Update(float dt) override;

private:
    // Ability Logic
    void DrainHealth(float damageDealt);

    // Ability Events
    void SetupCallbacks();
    void RegisterCallbacks();
    std::function<void(const GameObjectPtr& target, const sf::Vector2f& pos)> ResetProperties;
    std::function<void()> OnDeactivateCB;
    std::function<void()> OnCooldownCB;

private:
    float m_stackedDamage, m_damagePerStack;
    int m_buffStacks;
    float m_lifeDrainPerStack;

    // Components
    std::shared_ptr<class Health> m_health;
};