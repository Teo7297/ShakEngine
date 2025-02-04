#pragma once

#include "EngineDefines.h"
#include "Particle.h"
#include "GameObject.h"

namespace shak
{
    class ParticleSystem : public GameObject
    {
    public:
        ParticleSystem(int maxParticles = 50, float particlesPerSecond = 10.f, float minLifeTime = 0.5f, float maxLifeTime = 3.f, float initialDelay = 0.f, float minSize = 2.f, float maxSize = 15.f, sf::Vector2f minVelocity = { 0.f, 0.f }, sf::Vector2f maxVelocity = { 0.f, 50.f }, sf::Color startColor = sf::Color::White, sf::Color endColor = sf::Color::White, bool fade = false, Particle::Type type = Particle::Type::Point, std::shared_ptr<sf::Texture> texture = nullptr);
        ~ParticleSystem() override = default;

        void SetSpawnActive(bool active) { m_spawnActive = active; }

        void Update(float dt) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

    private:
        void InitParticle(int index);

    private:
        std::vector<Particle> m_particles;
        int m_maxParticles;
        float m_spawnRate;
        float m_spawnTimer;
        float m_minLifeTime, m_maxLifeTime;
        float m_minSize, m_maxSize;
        sf::Vector2f m_minVelocity, m_maxVelocity;
        sf::Color m_startColor, m_endColor;
        bool m_fade;
        bool m_spawnActive = true;
    };
}