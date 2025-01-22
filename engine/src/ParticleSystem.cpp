#include "ParticleSystem.h"

namespace shak
{
    ParticleSystem::ParticleSystem(int maxParticles, float spawnRate, float minLifeTime, float maxLifeTime, float minSize, float maxSize, sf::Vector2f minVelocity, sf::Vector2f maxVelocity, sf::Color startColor, sf::Color endColor, std::shared_ptr<sf::Texture> texture)
        :
        GameObject{ std::make_shared<sf::VertexArray>(sf::PrimitiveType::Triangles, maxParticles * 6 /*quad*/), texture }, m_maxParticles{ maxParticles },
        m_spawnRate{ spawnRate },
        m_spawnTimer{ 0.f },
        m_minLifeTime{ minLifeTime },
        m_maxLifeTime{ maxLifeTime },
        m_minSize{ minSize },
        m_maxSize{ maxSize },
        m_minVelocity{ minVelocity },
        m_maxVelocity{ maxVelocity },
        m_startColor{ startColor },
        m_endColor{ endColor }
    {
        m_particles.resize(m_maxParticles);

        for (int i = 0; i < m_maxParticles; i++)
        {
            int j = i * 6;
            m_particles[i].v0 = &(*m_vertices)[j];
            m_particles[i].v1 = &(*m_vertices)[j + 1];
            m_particles[i].v2 = &(*m_vertices)[j + 2];
            m_particles[i].v3 = &(*m_vertices)[j + 3];
            m_particles[i].v4 = &(*m_vertices)[j + 4];
            m_particles[i].v5 = &(*m_vertices)[j + 5];

            InitParticle(i);
        }
    }

    ParticleSystem::~ParticleSystem()
    {
    }

    void ParticleSystem::Update(float dt)
    {
        for (std::size_t i = 0; i < m_particles.size(); i++)
        {
            // update the particle lifetime
            Particle& p = m_particles[i];
            p.lifeTime += dt;

            // if the particle is dead, respawn it
            if (p.lifeTime >= p.maxLifeTime)
            {
                InitParticle(i);
            }

            // update the position of the corresponding vertex
            p.v0->position += (p.velocity * dt);
            p.v1->position += (p.velocity * dt);
            p.v2->position += (p.velocity * dt);
            p.v3->position += (p.velocity * dt);
            p.v4->position += (p.velocity * dt);
            p.v5->position += (p.velocity * dt);

            p.v0->color = p.startColor;
            p.v1->color = p.startColor;
            p.v2->color = p.startColor;
            p.v3->color = p.startColor;
            p.v4->color = p.startColor;
            p.v5->color = p.startColor;

            // update the alpha (transparency) of the particle according to its lifetime
            float ratio = 1 - (p.lifeTime / p.maxLifeTime) * 255;
            p.v0->color.a = static_cast<std::uint8_t>(ratio);
            p.v1->color.a = static_cast<std::uint8_t>(ratio);
            p.v2->color.a = static_cast<std::uint8_t>(ratio);
            p.v3->color.a = static_cast<std::uint8_t>(ratio);
            p.v4->color.a = static_cast<std::uint8_t>(ratio);
            p.v5->color.a = static_cast<std::uint8_t>(ratio);

            // texture coords
            float w = m_texture->getSize().x;
            float h = m_texture->getSize().y;
            p.v0->texCoords = { 0.f, 0.f };
            p.v1->texCoords = { 0.f, h };
            p.v2->texCoords = { w, 0.f };
            p.v3->texCoords = { w, 0.f };
            p.v4->texCoords = { 0.f, h };
            p.v5->texCoords = { w, h };
        }
    }

    void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::RenderStates ds = sf::RenderStates::Default;
        // apply the transform
        // states.transform *= getTransform();

        // our particles don't use a texture
        ds.texture = m_texture.get();

        // draw the vertex array
        target.draw(*m_vertices, ds);
    }

    void ParticleSystem::InitParticle(int index)
    {
        auto& p = m_particles[index];

        p.velocity = sf::Vector2f(
            m_minVelocity.x + (rand() % static_cast<int>(m_maxVelocity.x - m_minVelocity.x + 1)),
            m_minVelocity.y + (rand() % static_cast<int>(m_maxVelocity.y - m_minVelocity.y + 1))
        );

        auto psize = (m_minSize + (rand() % static_cast<int>(m_maxSize - m_minSize + 1))) / 2;
        p.v0->position = this->getPosition() + sf::Vector2f{ -psize, -psize };
        p.v1->position = this->getPosition() + sf::Vector2f{ -psize, psize };
        p.v2->position = this->getPosition() + sf::Vector2f{ psize, -psize };
        p.v3->position = this->getPosition() + sf::Vector2f{ psize, -psize };
        p.v4->position = this->getPosition() + sf::Vector2f{ -psize, psize };
        p.v5->position = this->getPosition() + sf::Vector2f{ psize, psize };

        p.lifeTime = 0.f;

        p.maxLifeTime = m_minLifeTime + (rand() % static_cast<int>(m_maxLifeTime - m_minLifeTime + 1));

        p.startColor =
        {
            static_cast<unsigned char>(m_startColor.r + (rand() % static_cast<int>(m_endColor.r - m_startColor.r + 1))),
            static_cast<unsigned char>(m_startColor.g + (rand() % static_cast<int>(m_endColor.g - m_startColor.g + 1))),
            static_cast<unsigned char>(m_startColor.b + (rand() % static_cast<int>(m_endColor.b - m_startColor.b + 1)))
        };

        p.endColor = p.startColor;
    }
}
