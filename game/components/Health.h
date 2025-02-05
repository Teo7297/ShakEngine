#pragma once

#include "ShakEngine.h"
#include "Component.h"
#include "ShakEvent.h"

class Health : public shak::Component
{
public:
    Health(shak::GameObject* owner)
        : shak::Component(owner)
    {
    }
    ~Health() override = default;

    void Awake() override;

    // TODO: Understand why inline causes linker errors
    void SetHealth(float health);
    float GetHealth() const;
    void SetMaxHealth(float maxHealth);
    float GetMaxHealth() const;

    void TakeDamage(float damage);
    void Heal(float amount);

    shak::Event<void> OnDeath;
    shak::Event<void, float> OnDamage;
    shak::Event<void, float> OnHeal;

private:
    float m_health, m_maxHealth = 100000.f;

};