#pragma once

#include "ShakEngine.h"
#include "Component.h"
#include "ShakEvent.h"

class Ability;
class AbilitySystem : public shak::Component
{
public:
    AbilitySystem(shak::GameObject* owner)
        : shak::Component(owner)
    {
    }
    virtual ~AbilitySystem() override = default;

    void Update(float dt) override;

    template<typename T>
    void AddAbility(const std::string& abilityName)
    {
        m_abilities[abilityName] = std::make_shared<T>();
        OnAbilityAdded(abilityName);
    }

    void RemoveAbility(const std::string& abilityName);
    void UseAbility(const std::string& abilityName);

    shak::Event<const std::string&> OnAbilityAdded;
    shak::Event<const std::string&> OnAbilityRemoved;
    shak::Event<const std::string&> OnAbilityUsed;

protected:
    std::unordered_map<std::string, std::shared_ptr<Ability>> m_abilities;
};