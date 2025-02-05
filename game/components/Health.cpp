#include "Health.h"

void Health::Awake()
{
    m_health = m_maxHealth;
}

void Health::SetHealth(float health)
{
    m_health = health;
}

float Health::GetHealth() const
{
    return m_health;
}

inline void Health::SetMaxHealth(float maxHealth)
{
    m_maxHealth = maxHealth;
}

inline float Health::GetMaxHealth() const
{
    return m_maxHealth;
}

void Health::TakeDamage(float damage)
{
    if (damage > m_health)
        damage = m_health;

    m_health -= damage;

    if (OnDamage)
        OnDamage(damage);

    if (m_health > 0.f) return;

    if (OnDeath)
        OnDeath();
}

void Health::Heal(float amount)
{
    if (amount + m_health > m_maxHealth)
        amount = m_maxHealth - m_health;
    m_health += amount;

    if (OnHeal)
        OnHeal(amount);
}

