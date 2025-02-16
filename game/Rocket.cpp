#include "Rocket.h"
#include "TrailRenderer.h"

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
    auto trail = std::make_shared<shak::TrailRenderer>(shak::TrailRenderer::TrailType::Line);
    trail->setPosition(this->getPosition());
    trail->SetZIndex(this->GetZIndex() - 10);
    trail->SetTTL(1.f);
    trail->SetFade(true);
    trail->SetColors(sf::Color::White, sf::Color::Green);
    this->AddChild(trail);
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
