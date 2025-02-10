#include "Ship.h"
#include "components/Health.h"

#define _USE_MATH_DEFINES
#include <math.h>

Ship::Ship(const std::shared_ptr<shak::TextureAtlas> atlas, const std::vector<sf::Vector2f> lasersOffsets, const std::shared_ptr<shak::TextureAtlas> deathAnimation)
    : GameObject(),
    m_atlas(atlas),
    m_atlasTexturesCount{ atlas->GetCount() },
    m_direction{ 1.f, 0.f }, // spawn looking right
    m_lookDirection{ 1.f, 0.f }, // spawn looking right
    m_lookAngle{ sf::Angle::Zero },
    m_distanceToDestination{ 0.f },
    m_destination{ 0.f, 0.f },
    m_damageNumberPool(20),
    m_laserShotPool(20),
    m_target{ nullptr },
    m_lasers{ },
    m_laserOffsets{ lasersOffsets },
    m_laserIndex{ 0 },
    m_speed{ 1000.f },
    m_damage{ 1000.f },
    m_deathAnimation{ std::make_shared<shak::Animation>(deathAnimation, 2.f) }
{
    auto coords = atlas->GetTextureCoords(1);
    m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
    (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
    (*m_vertices)[1].position = sf::Vector2f(0.f, 250.f);
    (*m_vertices)[2].position = sf::Vector2f(250.f, 0.f);
    (*m_vertices)[3].position = sf::Vector2f(250.f, 250.f);
    (*m_vertices)[0].color = sf::Color::White;
    (*m_vertices)[1].color = sf::Color::White;
    (*m_vertices)[2].color = sf::Color::White;
    (*m_vertices)[3].color = sf::Color::White;
    (*m_vertices)[0].texCoords = coords.topLeft;
    (*m_vertices)[1].texCoords = coords.bottomLeft;
    (*m_vertices)[2].texCoords = coords.topRight;
    (*m_vertices)[3].texCoords = coords.bottomRight;

    this->SetTexture(atlas->GetAtlasTexture());
    m_angleBetweenTextures = sf::degrees(360.f / static_cast<float>(m_atlasTexturesCount));

    auto origin = m_vertices->getBounds().getCenter();
    this->setOrigin(origin);
    // this->rotate(sf::degrees(-5));

    // Setup child points
    for (int i = 0; i < lasersOffsets.size(); i++)
    {
        auto laser = std::make_shared<shak::GameObject>();
        laser->setPosition(this->getPosition() + lasersOffsets[i]);
        m_lasers.push_back(laser);
        this->AddChild(laser);
    }

    // Setup death animation
    this->AddChild(m_deathAnimation);
    m_deathAnimation->SetFollowParent(false);

    this->EnablePhysics();
}

void Ship::Awake()
{
    m_destination = this->getPosition();

    auto aimtxt = m_engine->GetResourceManager().LoadTexture("assets/textures/aim.png", "aim");
    m_aimSprite = std::make_shared<shak::Sprite>(aimtxt, nullptr, sf::Vector2f(250.f, 250.f), sf::Color::Red);
    m_aimSprite->setPosition(this->getPosition());
    m_aimSprite->SetTransparency(0);
    m_aimSprite->SetZIndex(0);
    this->AddChild(m_aimSprite);


    std::shared_ptr<Health> health = this->AddComponent<Health>();
    health->OnDamage.Add(std::bind(&Ship::SpawnDamageNumber, this, std::placeholders::_1));
    health->OnDeath.Add(std::bind(&Ship::DisableAimSprite, this));
    health->OnDeath.Add(std::bind(&Ship::PlayDeathAnimation, this));
    health->OnDeath.Add(std::bind(&Ship::ResetHealth, this));
}

void Ship::Update(float dt)
{
    if (m_distanceToDestination > 10.f)
    {
        const auto offset = m_direction * m_speed * dt;
        this->move(offset);
        m_distanceToDestination -= offset.length();
    }
}

int Ship::GetTextureByDirection() const
{
    float signedAngle = m_lookAngle.asRadians() / (2.0f * static_cast<float>(M_PI));
    if (signedAngle < 0.f)
        signedAngle += 1.f;
    return static_cast<int>(signedAngle * static_cast<float>(m_atlasTexturesCount)) % m_atlasTexturesCount;
}

void Ship::UpdateDirection()
{
    const auto dir = m_destination - getPosition();
    auto len2 = dir.lengthSquared();
    if (len2 > 0.f)
    {
        m_distanceToDestination = std::sqrt(len2);   // Length using already computed dot
        m_direction = dir / m_distanceToDestination; // Normalize using already computed length
    }
}

void Ship::UpdateLookDirection()
{
    auto prevLookDirection = m_lookDirection;
    if (m_lookAtTarget && m_target)
        m_lookDirection = m_target->getPosition() - getPosition();
    else
        m_lookDirection = m_direction;

    m_lookAngle = -m_lookDirection.angle();
    this->setRotation(-(m_lookAngle % m_angleBetweenTextures));

    sf::Angle lasersAngle = prevLookDirection.angleTo(m_lookDirection);
    for (auto& laser : m_lasers)
        laser->rotateAround(lasersAngle, this->getPosition());
}

void Ship::UpdateTextureCoords()
{
    auto coords = m_atlas->GetTextureCoords(GetTextureByDirection());
    (*m_vertices)[0].texCoords = coords.topLeft;
    (*m_vertices)[1].texCoords = coords.bottomLeft;
    (*m_vertices)[2].texCoords = coords.topRight;
    (*m_vertices)[3].texCoords = coords.bottomRight;
}

void Ship::SetTarget(const std::shared_ptr<Ship>& target)
{
    m_target = target;
}

void Ship::ToggleAimSprite(bool show)
{
    m_aimSprite->SetTransparency(show ? 255 : 0);
}

void Ship::OnLaserHit()
{
    if (!m_target)
        return;

    auto health = m_target->GetComponent<Health>();
    float dmg = m_damage + std::rand() % 10000;
    if (health)
    {
        float dealtDamage = health->TakeDamage(dmg);
    }
}

// Events
void Ship::SpawnDamageNumber(float damage)
{
    auto damageNumber = m_damageNumberPool.Get();
    damageNumber->Reset(static_cast<int>(damage), this->getPosition());
    m_engine->GetScene()->AddGameObject(damageNumber);
}

void Ship::DisableAimSprite()
{
    ToggleAimSprite(false);
}

void Ship::PlayDeathAnimation()
{
    m_deathAnimation->setPosition(this->getPosition());
    m_deathAnimation->Play();
}

void Ship::ResetHealth()
{
    auto health = this->GetComponent<Health>();
    if (health)
    {
        health->SetHealth(health->GetMaxHealth());
    }
}

