#pragma once

#include "EngineDefines.h"
#include "Particle.h"
#include "GameObject.h"

namespace shak
{
    class ParticleSystem : public GameObject
    {
    public:
        ParticleSystem(Particle::Type type = Particle::Type::Point, int maxParticles = 50, float initialDelay = 0.f);
        ~ParticleSystem() override = default;

        void SetSpawnActive(bool active) { m_spawnActive = active; }

        void Update(float dt) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

        // Properties' setters
        void SetSpawnRate(float particlesPerSecond) { m_spawnRate = 1.f / particlesPerSecond; }
        void SetLifeTimes(float minLifeTime, float maxLifeTime) { m_minLifeTime = minLifeTime; m_maxLifeTime = maxLifeTime; }
        void SetSizes(float minSize, float maxSize) { m_minSize = minSize; m_maxSize = maxSize; }
        void SetDirections(sf::Vector2f minDir, sf::Vector2f maxDir) { m_minDir = minDir; m_maxDir = maxDir; }
        void SetSpeeds(float minSpeed, float maxSpeed) { m_minSpeed = minSpeed; m_maxSpeed = maxSpeed; }
        void SetColors(sf::Color startColor, sf::Color endColor) { m_startColor = startColor; m_endColor = endColor; }
        void SetFade(bool fade) { m_fade = fade; }

    private:
        void InitParticle(int index);

    private:
        std::vector<Particle> m_particles;
        int m_maxParticles;
        float m_spawnRate;
        float m_spawnTimer;
        float m_minLifeTime, m_maxLifeTime;
        float m_minSize, m_maxSize;
        sf::Vector2f m_minDir, m_maxDir;
        float m_minSpeed, m_maxSpeed;
        sf::Color m_startColor, m_endColor;
        bool m_fade;
        bool m_spawnActive;
    };
}