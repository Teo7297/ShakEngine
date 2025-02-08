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

    float TakeDamage(float damage);
    float Heal(float amount);

    shak::Event<> OnDeath;
    shak::Event<float> OnDamage;
    shak::Event<float> OnHeal;

private:
    float m_health, m_maxHealth = 100000.f;

};