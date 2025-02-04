#include "ParticleSystem.h"

namespace shak
{
    ParticleSystem::ParticleSystem(int maxParticles, float particlesPerSecond, float minLifeTime, float maxLifeTime, float initialDelay, float minSize, float maxSize, sf::Vector2f minVelocity, sf::Vector2f maxVelocity, sf::Color startColor, sf::Color endColor, bool fade, Particle::Type type, std::shared_ptr<sf::Texture> texture)
        :
        m_maxParticles{ maxParticles },
        m_spawnRate{ 1.f / particlesPerSecond },
        m_spawnTimer{ -initialDelay },
        m_minLifeTime{ minLifeTime },
        m_maxLifeTime{ maxLifeTime },
        m_minSize{ minSize },
        m_maxSize{ maxSize },
        m_minVelocity{ minVelocity },
        m_maxVelocity{ maxVelocity },
        m_startColor{ startColor },
        m_endColor{ endColor },
        m_fade{ fade }
    {
        if (type == Particle::Type::Quad)
        {
            m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::Triangles, maxParticles * 6);
            m_texture = texture;
        }
        else // Point
            m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::Points, maxParticles * 1);

        m_particles.resize(m_maxParticles, Particle{ type });

        for (int i = 0; i < m_maxParticles; i++)
        {
            m_particles[i].id = i;
            m_particles[i].SetVertices(m_vertices, i);
            InitParticle(i);
        }
    }

    void ParticleSystem::Update(float dt)
    {
        // stop spawn timer if the system is not active
        if (m_spawnActive)

            m_spawnTimer += dt;
        if (m_shader)
            m_shader->setUniform("u_time", m_spawnTimer);

        for (std::size_t i = 0; i < m_particles.size(); i++)
        {
            Particle& p = m_particles[i];
            p.lifeTime += dt;

            // if the particle is dead, respawn it
            if (p.lifeTime >= p.maxLifeTime)
                InitParticle(static_cast<int>(i));

            if (!p.active)
            {
                // Spawn a new particle (if the system is active)
                if (m_spawnActive && m_spawnTimer >= m_spawnRate)
                {
                    p.active = true;
                    m_spawnTimer -= m_spawnRate; // This allows for multiple particles to spawn in each frame!
                    if (m_spawnTimer >= 100000.f)
                        m_spawnTimer = 100000.f; // cap this value to avoid overflow
                }
                else
                {
                    continue;
                }
            }



            // Update each vertex of the particle
            // position
            p.Move(p.velocity * dt);

            // base color
            p.SetColor(p.startColor);

            // fade alpha over lifetime
            if (m_fade)
            {
                float ratio = 1 - (p.lifeTime / p.maxLifeTime) * 255;
                p.SetAlpha(static_cast<std::uint8_t>(ratio));
            }

            // texture coords
            if (!m_texture) continue;
            float w = static_cast<float>(m_texture->getSize().x);
            float h = static_cast<float>(m_texture->getSize().y);
            p.SetTextureCoords(w, h);
        }

    }

    void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::RenderStates ds = sf::RenderStates::Default;

        ds.texture = m_texture.get();
        ds.shader = m_shader.get();
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
        p.SetSize(psize);
        p.Move(this->getPosition());

        p.lifeTime = 0.f;

        p.maxLifeTime = m_minLifeTime + (rand() % static_cast<int>(m_maxLifeTime - m_minLifeTime + 1));

        p.startColor =
        {
            static_cast<unsigned char>(m_startColor.r + (rand() % static_cast<int>(m_endColor.r - m_startColor.r + 1))),
            static_cast<unsigned char>(m_startColor.g + (rand() % static_cast<int>(m_endColor.g - m_startColor.g + 1))),
            static_cast<unsigned char>(m_startColor.b + (rand() % static_cast<int>(m_endColor.b - m_startColor.b + 1)))
        };

        p.endColor = p.startColor;

        p.SetAlpha(0);

        p.active = false;
    }
}
