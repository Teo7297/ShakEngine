#pragma once

#include "Particle.h"

namespace shak
{
    class ParticleLogic
    {
    public:
        ParticleLogic() = default;
        virtual ~ParticleLogic() = default;

        virtual void OnParticleSpawn(Particle& particle) {};

        virtual void OnParticleUpdate(Particle& particle, float dt) {};

        virtual void OnParticleReset(Particle& particle) {};

        void OverrideMovement(bool override)
        {
            m_overrideMovement = override;
        }

        bool IsMovementOverridden() const
        {
            return m_overrideMovement;
        }

        void OverrideColor(bool override)
        {
            m_overrideColor = override;
        }

        bool IsColorOverridden() const
        {
            return m_overrideColor;
        }

        void OverrideFading(bool override)
        {
            m_overrideFading = override;
        }

        bool IsFadingOverridden() const
        {
            return m_overrideFading;
        }

    protected:
        bool m_overrideMovement = false;
        bool m_overrideColor = false;
        bool m_overrideFading = false;

    };
}