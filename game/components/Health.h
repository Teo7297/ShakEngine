#pragma once

#include "ShakEngine.h"
#include "Component.h"
#include "ShakEvent.h"

class Health : public shak::Component
{
public:
    Health(shak::GameObject* owner)
        : shak::Component(owner)
        , m_health(0.f)
        , m_maxHealth(0.f)
    {
    }
    ~Health() override = default;

    void Awake() override;

    void SetHealth(float health);
    float GetHealth() const;
    void SetMaxHealth(float maxHealth, bool resetHealth = true);
    float GetMaxHealth() const;

    float TakeDamage(float damage);
    float Heal(float amount);

    shak::Event<> OnDeath;
    shak::Event<float> OnDamage;
    shak::Event<float> OnHeal;

private:
    float m_health, m_maxHealth;

};