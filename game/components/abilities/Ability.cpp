#include "components/abilities/Ability.h"
#include "components/AbilitySystem.h"
#include "Ship.h"
#include "components/Energy.h"
#include "Ability.h"


Ability::Ability(const std::string& name, float cooldown, float duration, float activationCost, Type type, AbilitySystem* abilitySystem)
    : m_name(name)
    , m_cooldown(cooldown)
    , m_currentCooldown(0.0f)
    , m_duration(duration)
    , m_currentDuration(0.0f)
    , m_activationCost(activationCost)
    , m_isActive(type == Type::Passive)
    , m_type(type)
    , m_abilitySystem(abilitySystem)
{
    m_shipOwner = (Ship*)m_abilitySystem->GetOwner();
    m_energy = m_shipOwner->GetComponent<Energy>();
}

bool Ability::Toggle(const GameObjectPtr& target, const sf::Vector2f& targetPos)
{
    if (m_isActive)
    {
        Deactivate();
        return false;
    }
    else
    {
        Activate(target, targetPos);
        return true;
    }
}

void Ability::Activate(const GameObjectPtr& target, const sf::Vector2f& targetPos)
{
    if (m_currentCooldown > 0.0f)
        return;

    if (!m_energy->HasEngouhEnergy(m_activationCost))
        return;

    // Apply cost
    m_energy->UseEnergy(m_activationCost);

    if (m_duration > 0.f)
    {
        m_isActive = true;
        m_currentDuration = m_duration;
    }

    m_currentCooldown = m_cooldown;
    OnActivated(target, targetPos);
}

void Ability::Deactivate()
{
    if (!m_isActive)
        return;

    m_isActive = false;
    OnDeactivated();
}

void Ability::Update(float dt)
{
    if (m_type == Type::Active && m_currentCooldown > 0.0f)
    {
        m_currentCooldown -= dt;
        if (m_currentCooldown <= 0.0f)
        {
            m_currentCooldown = 0.0f;
            OnCooldown();
        }
    }

    if (m_type == Type::Active && m_isActive && m_currentDuration > 0.f)
    {
        m_currentDuration -= dt;
        if (m_currentDuration <= 0.f)
        {
            m_currentDuration = 0.0f;
            Deactivate();
        }
    }
}

void Ability::RestartCooldown()
{
    m_currentCooldown = m_cooldown;
}
