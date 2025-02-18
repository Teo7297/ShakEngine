#include "components/AbilitySystem.h"
#include "components/abilities/Ability.h"

void AbilitySystem::Update(float dt)
{
    for (auto& ability : m_abilities)
    {
        ability.second->Update(dt);
        ability.second->Ability::Update(dt);
    }
}

void AbilitySystem::RemoveAbility(const std::string& abilityName)
{
    m_abilities.erase(abilityName);
    OnAbilityRemoved(abilityName);
}

void AbilitySystem::UseAbility(const std::string& abilityName, const GameObjectPtr& target, const sf::Vector2f& targetPos)
{
    auto it = m_abilities.find(abilityName);
    if (it != m_abilities.end())
    {
        it->second->Activate(target, targetPos);
        OnAbilityUsed(abilityName);
    }
}

void AbilitySystem::ToggleAbility(const std::string& abilityName, const GameObjectPtr& target, const sf::Vector2f& targetPos)
{
    auto it = m_abilities.find(abilityName);
    if (it != m_abilities.end())
    {
        bool activated = it->second->Toggle(target, targetPos);
        if (activated)
            OnAbilityUsed(abilityName);
    }
}