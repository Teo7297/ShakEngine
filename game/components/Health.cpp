#include "Health.h"

void Health::Awake()
{
}

void Health::SetHealth(float health)
{
    m_health = health;
}

float Health::GetHealth() const
{
    return m_health;
}

void Health::SetMaxHealth(float maxHealth, bool resetHealth)
{
    m_maxHealth = maxHealth;
    if (resetHealth)
        m_health = maxHealth;
}

float Health::GetMaxHealth() const
{
    return m_maxHealth;
}

float Health::TakeDamage(float damage)
{
    if (damage > m_health)
        damage = m_health;

    m_health -= damage;

    if (OnDamage)
        OnDamage(damage);

    if (m_health <= 0.f && OnDeath)
        OnDeath();

    return damage;
}

float Health::Heal(float amount)
{
    if (amount + m_health > m_maxHealth)
        amount = m_maxHealth - m_health;
    m_health += amount;

    if (OnHeal)
        OnHeal(amount);

    return amount;
}

