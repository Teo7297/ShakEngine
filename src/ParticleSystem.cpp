#include "ParticleSystem.h"
#include "ShakEngine.h"
#include "MathExtensions.h"
#include "ParticleLogic.h"

namespace shak
{
    ParticleSystem::ParticleSystem(Particle::Type type, int maxParticles, float initialDelay)
        :
        m_particles{}
        , m_type{ type }
        , m_maxParticles{ maxParticles }
        , m_spawnRate{ 1.f / 10.f } // 10 particles per second
        , m_spawnTimer{ -initialDelay }
        , m_minLifeTime{ 0.5f }
        , m_maxLifeTime{ 3.f }
        , m_minSize{ 2.f }
        , m_maxSize{ 15.f }
        , m_pointSize{ 1.f }
        , m_minDir{ 0.f, 0.f }
        , m_maxDir{ 0.f, 0.f }
        , m_minSpeed{ 50.f }
        , m_maxSpeed{ 200.f }
        , m_startColor1{ sf::Color::White }
        , m_startColor2{ sf::Color::White }
        , m_endColor1{ sf::Color::White }
        , m_endColor2{ sf::Color::White }
        , m_fade{ false }
        , m_spawnActive{ true }
        , m_particleLogic{ nullptr }
        , m_trailEnabled{ false }
        , m_trailType{ Trail::TrailType::Strip }
        , m_trailWidthStart{ 1.f }
        , m_trailWidthEnd{ 1.f }
        , m_trailTTL{ 0.5f }
        , m_trailStartColor{ sf::Color::White }
        , m_trailEndColor{ sf::Color::White }
        , m_trailFade{ false }
        , m_emitterShapeResolution{ 10 }
        , m_emitterShapePoints{}
        , m_emitterShapeIndex{ 0 }
    {
        this->InitParticlesList();
    }

    void ParticleSystem::Update(float dt)
    {
        // stop spawn timer if the system is not active
        if(m_spawnActive)
            m_spawnTimer += dt;

        if(m_shader)
            m_shader->setUniform("u_time", ShakEngine::GetInstance().GetTime());

        for(std::size_t i = 0; i < m_particles.size(); i++)
        {
            Particle& p = m_particles[i];
            p.lifeTime += dt;

            if(m_trailEnabled)
                p.trail->UpdateTrail(dt, p.GetCenter());

            // if the particle is dead, reset it
            if(p.lifeTime >= p.maxLifeTime)
            {
                if(m_particleLogic)
                    m_particleLogic->OnParticleReset(p);
                InitParticle(static_cast<int>(i));
            }

            if(!p.active)
            {
                // Spawn a new particle (if the system is active)
                if(m_spawnActive && m_spawnTimer >= m_spawnRate)
                {
                    p.SetPosition(this->GetSpawnPoint());
                    p.velocity = p.velocity.rotatedBy(this->getRotation());
                    p.active = true;
                    m_spawnTimer -= m_spawnRate; // This allows for multiple particles to spawn in each frame!

                    if(m_trailEnabled)
                        p.trail->Clear();
                    if(m_spawnTimer >= 100000.f)
                        m_spawnTimer = 100000.f; // cap this value to avoid overflow
                    if(m_particleLogic)
                        m_particleLogic->OnParticleSpawn(p);
                }
                else
                {
                    continue;
                }
            }

            if(m_particleLogic)
                m_particleLogic->OnParticleUpdate(p, dt);

            // Update each vertex of the particle
            // position
            if(!m_particleLogic || !m_particleLogic->IsMovementOverridden())
                p.Move(p.velocity * dt);


            // base color
            if(!m_particleLogic || !m_particleLogic->IsColorOverridden())
                p.SetColor(shak::lerp(p.startColor, p.endColor, p.lifeTime / p.maxLifeTime));

            // fade alpha over lifetime
            if(m_fade && (!m_particleLogic || !m_particleLogic->IsFadingOverridden()))
            {
                float ratio = 1 - (p.lifeTime / p.maxLifeTime) * 255;
                p.SetAlpha(static_cast<std::uint8_t>(ratio));
            }

            // texture coords
            if(!m_texture) continue;
            float w = static_cast<float>(m_texture->getSize().x);
            float h = static_cast<float>(m_texture->getSize().y);
            p.SetTextureCoords(w, h);
        }
    }

    void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::RenderStates ds = sf::RenderStates::Default;

        if(m_type == Particle::Type::Quad)
        {
            ds.texture = m_texture.get();
            ds.shader = m_shader.get();
            target.draw(*m_vertices, ds);
        }
        else // Point
        {
            glCheck(glPointSize(m_pointSize));
            target.draw(*m_vertices, ds);
            glCheck(glPointSize(1.f));
        }


        if(m_trailEnabled)
            for(const auto& p : m_particles)
            {
                if(p.active)
                    p.trail->DrawTrail(target);
            }
    }

    void ParticleSystem::EnableTrail(Trail::TrailType type, float widthStart, float widthEnd, float ttl, sf::Color startColor, sf::Color endColor, bool fade)
    {
        m_trailEnabled = true;
        m_trailType = type;
        m_trailWidthStart = widthStart;
        m_trailWidthEnd = widthEnd;
        m_trailTTL = ttl;
        m_trailStartColor = startColor;
        m_trailEndColor = endColor;
        m_fade = fade;

        for(auto& p : m_particles)
        {
            auto trail = std::make_shared<Trail>(type);
            trail->SetTTL(ttl);
            trail->SetFade(fade);
            trail->SetWidths(widthStart, widthEnd);
            trail->SetColors(startColor, endColor);
            p.trail = trail;
        }
    }

    void ParticleSystem::EnableTrail()
    {
        m_trailEnabled = true;
        for(auto& p : m_particles)
        {
            if(!p.trail)
            {
                auto trail = std::make_shared<Trail>(m_trailType);
                trail->SetTTL(m_trailTTL);
                trail->SetFade(m_trailFade);
                trail->SetWidths(m_trailWidthStart, m_trailWidthEnd);
                trail->SetColors(m_trailStartColor, m_trailEndColor);
                p.trail = trail;
            }
        }
    }

    void ParticleSystem::DisableTrail()
    {
        m_trailEnabled = false;
        for(auto& p : m_particles)
        {
            if(p.trail)
            {
                p.trail->Clear();
                p.trail.reset();
            }
        }
    }

    void ParticleSystem::SetParticleType(Particle::Type type)
    {
        m_type = type;
        m_vertices->clear();
        m_particles.clear();
        InitParticlesList();
    }

    void ParticleSystem::SetMaxParticles(int maxParticles)
    {
        m_maxParticles = maxParticles;
        InitParticlesList();
    }

    void ParticleSystem::SetEmitterShapePoint()
    {
        this->SetEmitterShapeResolution(1);
        m_emitterShapeIndex = 0;
        m_emitterShapePoints[0].x = 0.f;
        m_emitterShapePoints[0].y = 0.f;
    }

    void ParticleSystem::SetEmitterShapeLine(const sf::Vector2f& start, const sf::Vector2f& end)
    {
        m_emitterShapePoints.clear();
        m_emitterShapeIndex = 0;

        auto step = (end - start).componentWiseDiv(sf::Vector2f{ (float)m_emitterShapeResolution, (float)m_emitterShapeResolution });
        sf::Vector2f current = start;
        for(int i = 0; i < m_emitterShapeResolution; i++)
        {
            m_emitterShapePoints.emplace_back(current);
            current += step;
        }
        std::shuffle(m_emitterShapePoints.begin(), m_emitterShapePoints.end(), std::default_random_engine(std::random_device{}()));
    }

    void ParticleSystem::SetEmitterShapeCircle(const float radius)
    {
        m_emitterShapePoints.clear();
        if(m_emitterShapeResolution < 3)
            m_emitterShapeResolution = 3; // Minimum resolution for a circle is 3 points
        m_emitterShapeIndex = 0;

        auto center = sf::Vector2f{ 0.f,0.f };

        // m_emitterShapePoints.emplace_back(center + sf::Vector2f{ std::cos(0.f) * radius, std::sin(0.f) * radius });
        for(int i = 0; i < m_emitterShapeResolution; i++)
        {
            const float angle = 2.f * (float)M_PI * (float)i / (float)(m_emitterShapeResolution);
            m_emitterShapePoints.emplace_back(center + sf::Vector2f{ std::cos(angle) * radius, std::sin(angle) * radius });
        }
        std::shuffle(m_emitterShapePoints.begin(), m_emitterShapePoints.end(), std::default_random_engine(std::random_device{}()));
    }

    void ParticleSystem::SetEmitterShapeResolution(const int res)
    {
        m_emitterShapeResolution = res;
        m_emitterShapePoints.resize(res);
        m_emitterShapeIndex = 0; // We also need to reset the index otherwise it will go out of bounds
    }

    // TODO: Use better random generation with these:
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_real_distribution<> dis(0.0, 1.0);
    void ParticleSystem::InitParticle(int index)
    {
        auto& p = m_particles[index];

        sf::Vector2f dir{ 0.f, 1.f };
        // If directions are equal, use a random direction
        if(m_minDir == m_maxDir) //! If this causes issues, we shoud use a EPSILON value to compare
            // Generate a random direction vector
            // Randomly generate x and y components between -1 and 1
            // Normalize the vector to get a direction
            // This will give us a random direction in the unit circle
        {
            dir = {
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.f - 1.f,
                static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.f - 1.f
            };
        }
        else
        {
            // Convert direction vectors to angles in degrees
            float minAngle = std::atan2(m_minDir.y, m_minDir.x) * 180.0f / M_PI;
            float maxAngle = std::atan2(m_maxDir.y, m_maxDir.x) * 180.0f / M_PI;

            // Ensure we use clockwise rotation from min to max
            if(maxAngle > minAngle) {
                maxAngle -= 360.0f; // This makes maxAngle < minAngle for clockwise rotation
            }

            // Get a random angle between min and max (in degrees)
            float randomRatio = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            float angle = minAngle + randomRatio * (maxAngle - minAngle);

            // Convert angle back to direction vector
            float angleRad = angle * M_PI / 180.0f;
            dir = { std::cos(angleRad), std::sin(angleRad) };
        }
        dir = dir.normalized();

        auto speed = m_minSpeed + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (m_maxSpeed - m_minSpeed);
        p.velocity = dir * speed;

        auto psize = m_minSize + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (m_maxSize - m_minSize);
        p.SetSize(psize / 2);
        p.Move(this->getPosition());

        p.lifeTime = 0.f;

        p.maxLifeTime = m_minLifeTime + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (m_maxLifeTime - m_minLifeTime);

        p.startColor = shak::lerp(m_startColor1, m_startColor2, static_cast<float>(rand()) / static_cast<float>(RAND_MAX));

        p.endColor = shak::lerp(m_endColor1, m_endColor2, static_cast<float>(rand()) / static_cast<float>(RAND_MAX));

        p.SetAlpha(0);

        p.active = false;

        if(m_trailEnabled)
            p.trail->Clear();
    }

    void ParticleSystem::InitParticlesList()
    {
        // Create the vertex array if needed
        if(!m_vertices)
            m_vertices = std::make_shared<sf::VertexArray>();

        // Resize the vertex array to hold the maximum number of particles
        if(m_type == Particle::Type::Quad)
        {
            m_vertices->setPrimitiveType(sf::PrimitiveType::Triangles);
            m_vertices->resize(m_maxParticles * 6);
        }
        else // Point
        {
            m_vertices->setPrimitiveType(sf::PrimitiveType::Points);
            m_vertices->resize(m_maxParticles * 1);
        }
        m_particles.resize(m_maxParticles, Particle{ m_type });

        // Initialize the particles
        for(int i = 0; i < m_maxParticles; i++)
        {
            m_particles[i].id = i;
            m_particles[i].SetVertices(m_vertices, i);
            m_particles[i].active = false;
        }

        if(m_trailEnabled)
            this->EnableTrail();
    }

    sf::Vector2f ParticleSystem::GetSpawnPoint()
    {
        auto pos = this->getPosition();
        auto res = pos + m_emitterShapePoints[m_emitterShapeIndex];
        m_emitterShapeIndex = (m_emitterShapeIndex + 1) % m_emitterShapePoints.size();
        return res;
    }
}
