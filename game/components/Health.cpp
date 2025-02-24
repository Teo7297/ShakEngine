#include "Health.h"

void Health::Awake()
{
    OnDamage += [this](float damage)
        {
            if (OnHealthChange)
                OnHealthChange(m_health, m_maxHealth);
        };

    OnCritDamage += [this](float damage)
        {
            if (OnHealthChange)
                OnHealthChange(m_health, m_maxHealth);
        };

    OnHeal += [this](float amount)
        {
            if (OnHealthChange)
                OnHealthChange(m_health, m_maxHealth);
        };
}

void Health::Update(float dt)
{
    m_regenTimer += dt;
    if (m_regenTimer >= m_regenCooldown)
    {
        m_regenTimer = 0.f;
        this->Heal(m_regenPerSecond);
    }
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

void Health::SetHealthRegen(float regenPerSecond)
{
    m_regenPerSecond = regenPerSecond;
}

float Health::GetHealthRegen() const
{
    return m_regenPerSecond;
}

float Health::TakeDamage(float damage, bool isCrit)
{
    if (!IsAlive())
        return 0.f;

    if (damage > m_health)
        damage = m_health;

    m_health -= damage;

    if (isCrit)
        OnCritDamage(damage);
    else
        OnDamage(damage);

    if (m_health <= 0.f && OnDeath)
        OnDeath();

    return damage;
}

float Health::Heal(float amount)
{
    if (!IsAlive())
        return 0.f;

    if (amount + m_health > m_maxHealth)
        amount = m_maxHealth - m_health;
    m_health += amount;

    if (OnHeal)
        OnHeal(amount);

    return amount;
}

bool Health::IsAlive() const
{
    return m_health > 0.f;
}

