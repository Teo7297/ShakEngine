#include "ParticleSystem.h"

namespace shak
{
    ParticleSystem::ParticleSystem(int maxParticles, float spawnRate, float minLifeTime, float maxLifeTime, float minSize, float maxSize, sf::Vector2f minVelocity, sf::Vector2f maxVelocity, sf::Color startColor, sf::Color endColor, std::shared_ptr<sf::Texture> texture)
        : m_maxParticles{ maxParticles }, m_spawnRate{ spawnRate }, m_spawnTimer{ 0.f }, m_minLifeTime{ minLifeTime }, m_maxLifeTime{ maxLifeTime }, m_minSize{ minSize }, m_maxSize{ maxSize }, m_minVelocity{ minVelocity }, m_maxVelocity{ maxVelocity }, m_startColor{ startColor }, m_endColor{ endColor }, m_particleTexture{ texture }
    {
        m_particles.resize(m_maxParticles, nullptr);

        for (auto& p : m_particles)
        {
            p = std::make_shared<Particle>();
            this->InitParticle(p, false);
            this->AddChild(p);
        }
    }

    ParticleSystem::~ParticleSystem()
    {
    }

    void ParticleSystem::Update(float dt)
    {
        m_spawnTimer += dt;
        if (m_spawnTimer >= m_spawnRate)
        {
            for (auto& p : m_particles)
            {
                if (!p->IsActive())
                {
                    this->InitParticle(p, true);
                    break;
                }
            }
            m_spawnTimer = 0.f;
        }

        for (const auto& child : m_children)
        {
            if (child->IsActive())
                child->Update(dt);
        }
    }

    void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= this->getTransform();
        for (const auto& child : m_children)
        {
            auto particle = std::dynamic_pointer_cast<Particle>(child);
            if (particle->IsActive())
            {
                auto states = sf::RenderStates::Default;
                if (particle->texture)
                    states.texture = particle->texture.get();

                child->draw(target, states);
            }
        }
    }

    void ParticleSystem::move(sf::Vector2f offset)
    {
        Transformable::move(offset);
    }

    void ParticleSystem::InitParticle(std::shared_ptr<Particle> p, bool activate)
    {
        p->setPosition(this->getPosition());
        p->lifeTime = 0.f;
        p->maxLifeTime = m_minLifeTime + (rand() % static_cast<int>(m_maxLifeTime - m_minLifeTime + 1));

        p->SetSize(m_minSize + (rand() % static_cast<int>(m_maxSize - m_minSize + 1)));

        p->velocity = sf::Vector2f(
            m_minVelocity.x + (rand() % static_cast<int>(m_maxVelocity.x - m_minVelocity.x + 1)),
            m_minVelocity.y + (rand() % static_cast<int>(m_maxVelocity.y - m_minVelocity.y + 1))
        );

        p->startColor =
        {
            static_cast<unsigned char>(m_startColor.r + (rand() % static_cast<int>(m_endColor.r - m_startColor.r + 1))),
            static_cast<unsigned char>(m_startColor.g + (rand() % static_cast<int>(m_endColor.g - m_startColor.g + 1))),
            static_cast<unsigned char>(m_startColor.b + (rand() % static_cast<int>(m_endColor.b - m_startColor.b + 1)))
        };

        if (m_particleTexture)
            p->SetTexture(m_particleTexture);

        p->SetActive(activate);
    }
}
