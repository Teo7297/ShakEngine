#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    struct Particle : public GameObject
    {
        Particle()
        {
            float size = 10.f;
            m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
            (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
            (*m_vertices)[1].position = sf::Vector2f(0.f, size);
            (*m_vertices)[2].position = sf::Vector2f(size, 0.f);
            (*m_vertices)[3].position = sf::Vector2f(size, size);
            (*m_vertices)[0].color = sf::Color::White;
            (*m_vertices)[1].color = sf::Color::White;
            (*m_vertices)[2].color = sf::Color::White;
            (*m_vertices)[3].color = sf::Color::White;
            // (*m_vertices)[0].texCoords = { 0.f, 0.f };
            // (*m_vertices)[1].texCoords = { 0.f, 100.f };
            // (*m_vertices)[2].texCoords = { 100.f, 0.f };
            // (*m_vertices)[3].texCoords = { 100.f, 100.f };
        }
        ~Particle() override {}

        void SetSize(float size)
        {
            (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
            (*m_vertices)[1].position = sf::Vector2f(0.f, size);
            (*m_vertices)[2].position = sf::Vector2f(size, 0.f);
            (*m_vertices)[3].position = sf::Vector2f(size, size);
        }

        void SetTexture(std::shared_ptr<sf::Texture> t)
        {
            texture = t;
            auto size = texture->getSize();
            (*m_vertices)[0].texCoords = { 0.f, 0.f };
            (*m_vertices)[1].texCoords = { 0.f, static_cast<float>(size.y) };
            (*m_vertices)[2].texCoords = { static_cast<float>(size.x), 0.f };
            (*m_vertices)[3].texCoords = { static_cast<float>(size.x), static_cast<float>(size.y) };
        }

        void Update(float dt) override
        {
            lifeTime += dt;
            if (lifeTime >= maxLifeTime)
            {
                this->SetActive(false);
                return;
            }

            this->move(velocity * dt);

            (*m_vertices)[0].color = startColor;
            (*m_vertices)[1].color = startColor;
            (*m_vertices)[2].color = startColor;
            (*m_vertices)[3].color = startColor;
        }

        std::shared_ptr<sf::Texture> texture;
        sf::Vector2f velocity;
        float lifeTime;
        float maxLifeTime;
        sf::Color startColor, endColor;
        float size;

    };

    class ParticleSystem : public GameObject
    {
    public:
        ParticleSystem(int maxParticles = 50, float spawnRate = 0.5f, float minLifeTime = 1.f, float maxLifeTime = 2.f, float minSize = 2.f, float maxSize = 15.f, sf::Vector2f minVelocity = { 0.f, 0.f }, sf::Vector2f maxVelocity = { 0.f, 50.f }, sf::Color startColor = sf::Color::White, sf::Color endColor = sf::Color::White, std::shared_ptr<sf::Texture> texture = nullptr);
        ~ParticleSystem() override;

        void Update(float dt) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
        void move(sf::Vector2f offset) override;

    private:
        void InitParticle(std::shared_ptr<Particle> p, bool activate);
    private:
        std::vector<std::shared_ptr<Particle>> m_particles;
        int m_maxParticles;
        float m_spawnRate;
        float m_spawnTimer;
        float m_minLifeTime, m_maxLifeTime;
        float m_minSize, m_maxSize;
        sf::Vector2f m_minVelocity, m_maxVelocity;
        sf::Color m_startColor, m_endColor;
        std::shared_ptr<sf::Texture> m_particleTexture;
    };
}