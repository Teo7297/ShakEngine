#include "ParticleSystem.h"
#include "ShakEngine.h"
#include "MathExtensions.h"

namespace shak
{
    ParticleSystem::ParticleSystem(Particle::Type type, int maxParticles, float initialDelay)
        :
        m_particles{},
        m_maxParticles{ maxParticles },
        m_spawnRate{ 1.f / 10.f }, // 10 particles per second
        m_spawnTimer{ -initialDelay },
        m_minLifeTime{ 0.5f },
        m_maxLifeTime{ 3.f },
        m_minSize{ 2.f },
        m_maxSize{ 15.f },
        m_minDir{ 1.f, 0.f },
        m_maxDir{ 0.f, -1.f },
        m_minSpeed{ 50.f },
        m_maxSpeed{ 200.f },
        m_startColor{ sf::Color::White },
        m_endColor{ sf::Color::White },
        m_fade{ false },
        m_spawnActive{ true }
    {
        if (type == Particle::Type::Quad)
        {
            m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::Triangles, m_maxParticles * 6);
        }
        else // Point
            m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::Points, m_maxParticles * 1);

        m_particles.resize(m_maxParticles, Particle{ type });

        for (int i = 0; i < m_maxParticles; i++)
        {
            m_particles[i].id = i;
            m_particles[i].SetVertices(m_vertices, i);
            m_particles[i].active = false;
        }
    }

    void ParticleSystem::Update(float dt)
    {
        // stop spawn timer if the system is not active
        if (m_spawnActive)
            m_spawnTimer += dt;

        if (m_shader)
            m_shader->setUniform("u_time", ShakEngine::GetInstance().GetTime());
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
                    p.SetPosition(this->getPosition());
                    p.velocity = p.velocity.rotatedBy(this->getRotation());
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

    // TODO: Use better random generation with these:
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_real_distribution<> dis(0.0, 1.0);
    void ParticleSystem::InitParticle(int index)
    {
        auto& p = m_particles[index];

        auto dir = shak::slerp(m_minDir, m_maxDir, static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
        auto speed = m_minSpeed + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (m_maxSpeed - m_minSpeed);
        p.velocity = dir * speed;

        auto psize = m_minSize + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (m_maxSize - m_minSize);
        p.SetSize(psize / 2);
        p.Move(this->getPosition());

        p.lifeTime = 0.f;

        p.maxLifeTime = m_minLifeTime + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (m_maxLifeTime - m_minLifeTime);

        p.startColor = shak::lerp(m_startColor, m_endColor, static_cast<float>(rand()) / static_cast<float>(RAND_MAX));

        p.endColor = p.startColor;

        p.SetAlpha(0);

        p.active = false;
    }
}
