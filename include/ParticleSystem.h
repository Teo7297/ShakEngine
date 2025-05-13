#pragma once

#include "EngineDefines.h"
#include "Particle.h"
#include "GameObject.h"

namespace shak
{
    class ParticleLogic;
    class ParticleSystem : public GameObject
    {
    public:
        ParticleSystem(Particle::Type type = Particle::Type::Point, int maxParticles = 50, float initialDelay = 0.f);
        ~ParticleSystem() override = default;

        void SetSpawnActive(bool active) { m_spawnActive = active; }

        void Update(float dt) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

        void EnableTrail(Trail::TrailType type, float widthStart, float widthEnd, float ttl, sf::Color startColor, sf::Color endColor, bool fade);
        void EnableTrail();
        void DisableTrail();
        bool IsTrailEnabled() const { return m_trailEnabled; }

        //---------- Properties' setters -----------

        // This requires a reallocation of the vertex array and the particles list
        void SetMaxParticles(int maxParticles);
        void SetSpawnRate(float particlesPerSecond) { m_spawnRate = 1.f / particlesPerSecond; }
        void SetLifeTimes(float minLifeTime, float maxLifeTime) { m_minLifeTime = minLifeTime; m_maxLifeTime = maxLifeTime; }
        void SetSizes(float minSize, float maxSize) { m_minSize = minSize; m_maxSize = maxSize; }
        void SetDirections(sf::Vector2f minDir, sf::Vector2f maxDir) { m_minDir = minDir; m_maxDir = maxDir; }
        void SetSpeeds(float minSpeed, float maxSpeed) { m_minSpeed = minSpeed; m_maxSpeed = maxSpeed; }
        void SetStartColors(sf::Color startColor1, sf::Color startColor2) { m_startColor1 = startColor1; m_startColor2 = startColor2; }
        void SetEndColors(sf::Color endColor1, sf::Color endColor2) { m_endColor1 = endColor1; m_endColor2 = endColor2; }
        void SetFade(bool fade) { m_fade = fade; }
        void SetEmitterShapePoint();
        void SetEmitterShapeLine(const sf::Vector2f& start, const sf::Vector2f& end);
        void SetEmitterShapeCircle(const float radius);
        void SetEmitterShapeResolution(const int res);

        //---------- Properties' getters -----------
        int GetMaxParticles() const { return m_maxParticles; }
        float GetSpawnRate() const { return 1.f / m_spawnRate; }
        float GetMinLifeTime() const { return m_minLifeTime; }
        float GetMaxLifeTime() const { return m_maxLifeTime; }
        float GetMinSize() const { return m_minSize; }
        float GetMaxSize() const { return m_maxSize; }
        sf::Vector2f GetMinDir() const { return m_minDir; }
        sf::Vector2f GetMaxDir() const { return m_maxDir; }
        float GetMinSpeed() const { return m_minSpeed; }
        float GetMaxSpeed() const { return m_maxSpeed; }
        std::pair<sf::Color, sf::Color> GetStartColors() const { return { m_startColor1, m_startColor2 }; }
        std::pair<sf::Color, sf::Color> GetEndColors() const { return { m_endColor1, m_endColor2 }; }
        int GetEmitterShapeResolution() const { return m_emitterShapeResolution; }

        bool GetFade() const { return m_fade; }
        Particle::Type GetType() const { return m_type; }

        // Custom particle logic
        template<typename T>
        void SetParticleLogic()
        {
            // Check if T is a subclass of ParticleLogic
            static_assert(std::is_base_of<ParticleLogic, T>::value, "T must be a subclass of ParticleLogic");

            // Create a new instance of T and assign it to m_particleLogic
            m_particleLogic = std::make_shared<T>();
        }

    private:
        void InitParticle(int index);
        void InitParticlesList();
        sf::Vector2f GetSpawnPoint();

    private:
        Particle::Type m_type;
        std::vector<Particle> m_particles;
        int m_maxParticles;
        float m_spawnRate;
        float m_spawnTimer;
        float m_minLifeTime, m_maxLifeTime;
        float m_minSize, m_maxSize;
        sf::Vector2f m_minDir, m_maxDir;
        float m_minSpeed, m_maxSpeed;
        sf::Color m_startColor1, m_startColor2, m_endColor1, m_endColor2;
        bool m_fade;
        bool m_spawnActive;
        std::shared_ptr<ParticleLogic> m_particleLogic;

        bool m_trailEnabled;
        Trail::TrailType m_trailType;
        float m_trailWidthStart, m_trailWidthEnd;
        float m_trailTTL;
        sf::Color m_trailStartColor, m_trailEndColor;
        bool m_trailFade;
        int m_emitterShapeResolution;
        std::vector<sf::Vector2f> m_emitterShapePoints;
        int m_emitterShapeIndex;
    };
}