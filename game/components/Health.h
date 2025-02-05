#pragma once

#include "ShakEngine.h"
#include "Component.h"
#include "ShakEvent.h"

class Health : public shak::Component
{
public:
    Health() = default;
    ~Health() override = default;

    void Awake() override;

    inline void SetHealth(float health);
    inline float GetHealth() const;
    inline void SetMaxHealth(float maxHealth);
    inline float GetMaxHealth() const;

    void TakeDamage(float damage);
    void Heal(float amount);

    shak::Event<void> OnDeath;
    shak::Event<void, float> OnDamage;
    shak::Event<void, float> OnHeal;

private:
    float m_health, m_maxHealth = 10000.f;

};