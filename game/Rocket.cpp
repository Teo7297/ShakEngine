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
    , m_target(nullptr)
{
    // Setup explosion animation
    auto explosionAtlas = shak::ShakEngine::GetInstance().GetResourceManager().LoadTextureAtlas("assets/animations/explosion.atlas", "deathExplosion");
    m_explosion = std::make_shared<shak::Animation>(explosionAtlas, 2.f);
    this->AddChild(m_explosion);
    m_explosion->SetFollowParent(false);
}

void Rocket::Init(const sf::Vector2f& start, const std::shared_ptr<Ship>& target)
{
    m_start = start;
    m_target = target;
    m_spline = std::make_unique<shak::Spline>(m_start, m_start + sf::Vector2f{ 0.f, -300.f }, m_start + sf::Vector2f{ 250.f, 400.f }, m_target->getPosition());
}

void Rocket::Awake()
{
    auto trail = std::make_shared<shak::TrailRenderer>(shak::TrailRenderer::TrailType::Strip);
    trail->setPosition(this->getPosition());
    // trail->SetShader(shak::ShakEngine::GetInstance().GetResourceManager().LoadShader("", "assets/shaders/particle.fs", "trail"));
    trail->SetZIndex(this->GetZIndex() - 10);
    trail->SetTTL(0.3f);
    trail->SetFade(true);
    trail->SetColors(sf::Color::Yellow, sf::Color::Red);
    trail->SetWidths(10.f, 10.f);
    this->AddChild(trail);

    auto back = sf::Vector2f{ 0, 1 };
    auto psMinDir = back.rotatedBy(sf::degrees(-10));
    auto psMaxDir = back.rotatedBy(sf::degrees(10));
    auto ps = std::make_shared<shak::ParticleSystem>(shak::Particle::Type::Point, 900, 0.f);
    ps->SetSpawnRate(300.f);
    ps->SetLifeTimes(0.2f, 0.5f);
    ps->SetDirections(psMinDir, psMaxDir);
    ps->SetSpeeds(100.f, 200.f);
    ps->SetColors(sf::Color::Red, sf::Color::Yellow);
    ps->SetFade(false);
    ps->setPosition(this->getPosition());
    this->AddChild(ps);
}

void Rocket::Update(float dt)
{
    // Update moving destination
    m_spline->UpdatePoint(3, m_target->getPosition());

    // Update position
    m_t += dt;
    if (m_t > 1.f)
    {
        m_explosion->setPosition(this->getPosition());
        m_explosion->Play();

        this->SetActive(false);
        m_t = 0.f;
        this->SetActive(true);
    }

    auto pos = m_spline->GetPoint(m_t);

    auto dir = (pos - m_previousPos).normalized();
    auto angle = dir.angle();
    angle += sf::degrees(90.f);
    this->setRotation(angle);

    m_previousPos = pos;
    this->setPosition(pos);
}
