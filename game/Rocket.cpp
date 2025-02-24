#include "Rocket.h"
#include "TrailRenderer.h"
#include "ParticleSystem.h"

Rocket::Rocket(const std::shared_ptr<sf::Texture>& texture, const std::shared_ptr<sf::Shader>& shader)
    : Sprite(texture, shader)
    , m_start(0.f, 0.f)
    , m_destination(0.f, 0.f)
    , m_spline(nullptr)
    , m_previousPos(this->getPosition())
    , m_t(0.f)
    , m_explosion(nullptr)
    , m_ps(nullptr)
    , m_target(nullptr)
{
    // Setup explosion animation
    auto explosionAtlas = shak::ShakEngine::GetInstance().GetResourceManager()->LoadTextureAtlas("assets/animations/explosion.atlas", "deathExplosion");
    m_explosion = m_engine->AddGameObject<shak::Animation>(explosionAtlas, 2.f);
}

void Rocket::Init(const sf::Vector2f& start, const std::shared_ptr<Ship>& target)
{
    m_start = start;
    m_target = target;
    auto dir = (m_target->getPosition() - m_start).normalized();
    auto perp = dir.rotatedBy(sf::degrees(90.f));
    bool invert = std::rand() % 2;
    float intensity = invert ? -300.f : 300.f;
    sf::Vector2f p1 = m_start + perp * intensity;
    sf::Vector2f p2 = m_target->getPosition() + perp * intensity;
    m_spline = std::make_unique<shak::Spline>(m_start, p1, p2, m_target->getPosition());
    if (m_ps)
        m_ps->SetSpawnActive(true);
    this->SetActive(true);
}

void Rocket::Awake()
{
    auto trail = std::make_shared<shak::TrailRenderer>(shak::TrailRenderer::TrailType::Strip);
    trail->setPosition(this->getPosition());
    // trail->SetShader(shak::ShakEngine::GetInstance().GetResourceManager->LoadShader("", "assets/shaders/particle.fs", "trail"));
    trail->SetZIndex(this->GetZIndex() - 10);
    trail->SetTTL(0.3f);
    trail->SetFade(true);
    trail->SetColors(sf::Color::Yellow, sf::Color::Red);
    trail->SetWidths(10.f, 10.f);
    this->AddChild(trail);

    auto back = sf::Vector2f{ 0, 1 };
    auto psMinDir = back.rotatedBy(sf::degrees(-10));
    auto psMaxDir = back.rotatedBy(sf::degrees(10));
    m_ps = m_engine->AddGameObject<shak::ParticleSystem>(shak::Particle::Type::Point, 900, 0.f);
    m_ps->SetSpawnRate(300.f);
    m_ps->SetLifeTimes(0.2f, 0.5f);
    m_ps->SetDirections(psMinDir, psMaxDir);
    m_ps->SetSpeeds(100.f, 200.f);
    m_ps->SetColors(sf::Color::Red, sf::Color::Yellow);
    m_ps->SetFade(false);
    m_ps->setPosition(this->getPosition());
    m_ps->SetSpawnActive(true);
}

void Rocket::Update(float dt)
{
    // Update moving destination
    m_spline->UpdatePoint(3, m_target->getPosition());

    // Update position
    m_t += dt;
    if (m_t > 1.f)
    {
        OnHit(m_target);
        m_explosion->setPosition(this->getPosition());
        m_explosion->Play();
        this->SetActive(false);
        m_ps->SetSpawnActive(false);
        m_t = 0.f;
    }

    auto pos = m_spline->GetPoint(m_t);

    auto dir = (pos - m_previousPos).normalized();
    auto angle = dir.angle();
    angle += sf::degrees(90.f);
    this->setRotation(angle);

    m_previousPos = pos;
    this->setPosition(pos);
    m_ps->setPosition(pos);
    m_ps->setRotation(this->getRotation());
}
