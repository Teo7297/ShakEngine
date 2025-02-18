#include "Energy.h"

void Energy::Awake()
{
}

void Energy::Update(float dt)
{
    m_regenTimer += dt;
    if (m_regenTimer >= m_regenCooldown)
    {
        m_regenTimer = 0.f;
        this->GainEnergy(m_regenPerSecond);
    }
}

void Energy::SetEnergy(float energy)
{
    m_energy = energy;
}

float Energy::GetEnergy() const
{
    return m_energy;
}

void Energy::SetMaxEnergy(float maxEnergy, bool resetEnergy)
{
    m_maxEnergy = maxEnergy;
    if (resetEnergy)
        m_energy = maxEnergy;
}

float Energy::GetMaxEnergy() const
{
    return m_maxEnergy;
}

void Energy::SetEnergyRegen(float regenPerSecond)
{
    m_regenPerSecond = regenPerSecond;
}

float Energy::GetEnergyRegen() const
{
    return m_regenPerSecond;
}

bool Energy::HasEngouhEnergy(float amount) const
{
    return m_energy >= amount;
}

float Energy::UseEnergy(float amount)
{
    float used = 0.0f;
    if (this->HasEngouhEnergy(amount))
    {
        m_energy -= amount;
        OnEnergyUsed(amount);
        used = amount;
    }
    std::cout << "Energy remaining: " << m_energy << std::endl;
    return used;
}

float Energy::GainEnergy(float amount)
{
    auto missing = m_maxEnergy - m_energy;
    float gained = 0.0f;
    if (missing >= amount)
    {
        m_energy += amount;
        gained = amount;
    }
    else
    {
        m_energy = m_maxEnergy;
        gained = missing;
    }

    OnEnergyGained(gained);

    return gained;
}
