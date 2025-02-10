#pragma once

#include "components/abilities/Ability.h"

class TestAbility : public Ability
{
public:
    TestAbility()
        : Ability("TestAbility", 3.0f, 2.0f, Type::Active)
    {
        OnActivated += ActivateCB;
        OnDeactivated += DeactivateCB;
        OnCooldown += CooldownCB;
    }

    void Update(float dt) override
    {
        if (!m_isActive)
            return;
        static int fcount = 0;
        if (fcount++ % 100 == 0)
            std::cout << "TestAbility updated" << std::endl;
    }

private:

    std::function<void()> ActivateCB = [this]()
        {
            std::cout << "TestAbility activated" << std::endl;
        };

    std::function<void()> DeactivateCB = [this]()
        {
            std::cout << "TestAbility deactivated" << std::endl;
        };

    std::function<void()> CooldownCB = [this]()
        {
            std::cout << "TestAbility's cooldown ready" << std::endl;
        };
};