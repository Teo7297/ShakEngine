#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    struct Particle
    {
        sf::Vertex* v0, * v1, * v2, * v3, * v4, * v5;
        sf::Vector2f velocity;
        float lifeTime;
        float maxLifeTime;
        sf::Color startColor, endColor;
    };

    class ParticleSystem : public GameObject
    {
    public:
        ParticleSystem(int maxParticles = 50, float spawnRate = 0.5f, float minLifeTime = 0.5f, float maxLifeTime = 3.f, float minSize = 2.f, float maxSize = 15.f, sf::Vector2f minVelocity = { 0.f, 0.f }, sf::Vector2f maxVelocity = { 0.f, 50.f }, sf::Color startColor = sf::Color::White, sf::Color endColor = sf::Color::White, std::shared_ptr<sf::Texture> texture = nullptr);
        ~ParticleSystem() override;

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
    };
}