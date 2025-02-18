#pragma once

#include "ShakEngine.h"
#include "Component.h"
#include "ShakEvent.h"

class Energy : public shak::Component
{
public:
    Energy(shak::GameObject* owner)
        : shak::Component(owner)
        , m_energy(0.f)
        , m_maxEnergy(0.f)
        , m_regenPerSecond(0.f)
        , m_regenTimer(0.f)
        , m_regenCooldown(1.f)
    {
    }
    ~Energy() override = default;

    void Awake() override;
    void Update(float dt) override;

    void SetEnergy(float energy);
    float GetEnergy() const;
    void SetMaxEnergy(float maxEnergy, bool resetEnergy = true);
    float GetMaxEnergy() const;

    void SetEnergyRegen(float regenPerSecond);
    float GetEnergyRegen() const;

    bool HasEngouhEnergy(float amount) const;
    float UseEnergy(float amount);
    float GainEnergy(float amount);

    shak::Event<float> OnEnergyUsed;
    shak::Event<float> OnEnergyGained;

private:
    float m_energy, m_maxEnergy;
    float m_regenPerSecond, m_regenTimer, m_regenCooldown;
};