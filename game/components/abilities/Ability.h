#pragma once

#include "ShakEngine.h"
#include "ShakEvent.h"

class AbilitySystem;
class Ship;
class Energy;
class Ability
{
protected:
    enum class Type
    {
        Active,
        Passive
    };

public:
    Ability(const std::string& name, float cooldown, float duration, float activationCost, Type type, AbilitySystem* abilitySystem);

    bool Toggle(const GameObjectPtr& target = nullptr, const sf::Vector2f& targetPos = { 0, 0 });

    void Activate(const GameObjectPtr& target = nullptr, const sf::Vector2f& targetPos = { 0, 0 });

    void Deactivate();

    virtual void Update(float dt);

    void RestartCooldown();

public:
    // EVENTS
    shak::Event<> OnCooldown;
    shak::Event<const GameObjectPtr&, const sf::Vector2f&> OnActivated;
    shak::Event<> OnDeactivated;

protected:
    std::string m_name;
    bool m_isActive;
    float m_cooldown, m_currentCooldown;
    float m_duration, m_currentDuration;
    float m_activationCost;
    Type m_type;
    AbilitySystem* m_abilitySystem;
    Ship* m_shipOwner;
    std::shared_ptr<Energy> m_energy;
};