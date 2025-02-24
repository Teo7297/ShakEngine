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
        , m_regenPerSecond(0.f)
        , m_regenTimer(0.f)
        , m_regenCooldown(1.f)
    {
    }
    ~Health() override = default;

    void Awake() override;
    void Update(float dt) override;

    void SetHealth(float health);
    float GetHealth() const;
    void SetMaxHealth(float maxHealth, bool resetHealth = true);
    float GetMaxHealth() const;

    void SetHealthRegen(float regenPerSecond);
    float GetHealthRegen() const;

    float TakeDamage(float damage, bool isCrit);
    float Heal(float amount);

    bool IsAlive() const;

    shak::Event<> OnDeath;
    shak::Event<float> OnDamage;
    shak::Event<float> OnCritDamage;
    shak::Event<float> OnHeal;
    shak::Event<float, float> OnHealthChange; // current, total

private:
    float m_health, m_maxHealth;
    float m_regenPerSecond, m_regenTimer, m_regenCooldown;
};