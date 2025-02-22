#include "Ship.h"
#include "components/Health.h"
#include "components/Energy.h"
#include "components/AbilitySystem.h"
#include "components/BuffList.h"
#include "components/HealthBar.h"

#include "shapes/Square.h"

#define _USE_MATH_DEFINES
#include <math.h>

Ship::Ship(const json::JSON& shipData)
    : GameObject(),
    m_controlledByPlayer{ false },
    m_atlas(nullptr),
    m_atlasTexturesCount{ 0 },
    m_direction{ 1.f, 0.f }, // spawn looking right
    m_lookDirection{ 1.f, 0.f }, // spawn looking right
    m_lookAngle{ sf::Angle::Zero },
    m_distanceToDestination{ 0.f },
    m_destination{ 0.f, 0.f },
    m_damageNumberPool(20),
    m_shipData{ },
    m_target{ nullptr },
    m_lasers{ },
    m_laserIndex{ 0 },
    m_lookAtTarget{ false },
    m_targetWasSelected{ false },
    m_laserShooting{ false },
    m_isAutoAttacking{ false },
    m_baseStats{ },
    m_speed{ 0.f },
    m_damage{ 0.f },
    m_deathAnimation{ nullptr }
{
    m_shipData = shipData;

    auto rm = m_engine->GetResourceManager();
    m_atlas = rm.LoadTextureAtlas(shipData.at("atlas").ToString(), shipData.at("atlas_name").ToString());
    m_atlasTexturesCount = m_atlas->GetCount();

    this->InitQuadVertexArray({ 250.f, 250.f }, sf::Color::White);

    this->SetTexture(m_atlas->GetAtlasTexture());
    m_angleBetweenTextures = sf::degrees(360.f / static_cast<float>(m_atlasTexturesCount));

    this->CenterOrigin();

    auto laser_offsets = shipData.at("laser_offsets").ArrayRange();
    for (const auto& offset : laser_offsets)
    {
        auto v2offset = sf::Vector2f(offset.at("x").ToFloat(), offset.at("y").ToFloat());
        auto laser = std::make_shared<shak::GameObject>();
        laser->setPosition(this->getPosition() + v2offset);
        m_lasers.push_back(laser);
        this->AddChild(laser);
    }

    m_baseStats = shipData.at("base_stats");

    // Setup death animation
    auto deathAtlas = rm.LoadTextureAtlas(shipData.at("death_anim").ToString(), shipData.at("death_anim_name").ToString());
    m_deathAnimation = std::make_shared<shak::Animation>(deathAtlas, 2.f);
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

    // Setup initial base stats
    m_speed = m_baseStats.at("speed").ToFloat();
    m_damage = m_baseStats.at("damage").ToFloat();

    auto maxHealth = m_baseStats.at("health").ToFloat();
    auto healthRegen = m_baseStats.at("health_regen").ToFloat();
    std::shared_ptr<Health> health = this->AddComponent<Health>();
    health->SetMaxHealth(maxHealth);
    health->SetHealthRegen(healthRegen);
    health->OnDamage += std::bind(&Ship::SpawnDamageNumber, this, std::placeholders::_1);
    health->OnCritDamage += std::bind(&Ship::SpawnCritDamageNumber, this, std::placeholders::_1);
    health->OnDeath += std::bind(&Ship::DisableAimSprite, this);
    health->OnDeath += std::bind(&Ship::PlayDeathAnimation, this);
    health->OnDeath += std::bind(&Ship::ResetHealth, this);

    this->AddComponent<AbilitySystem>();

    auto maxEnergy = m_baseStats.at("energy").ToFloat();
    auto energyRegen = m_baseStats.at("energy_regen").ToFloat();
    std::shared_ptr<Energy> energy = this->AddComponent<Energy>();
    energy->SetMaxEnergy(maxEnergy);
    energy->SetEnergyRegen(energyRegen);

    OnAutoAttackStarted += [this](const GameObjectPtr& target)
        {
            m_isAutoAttacking = true;
        };
    OnAutoAttackStopped += [this]()
        {
            m_isAutoAttacking = false;
        };

    this->AddComponent<BuffList>();
    this->AddComponent<HealthBar>();

    // DEBUG
    auto square = std::make_shared<shak::Square>(sf::FloatRect(this->getPosition(), this->GetVertexArray()->getBounds().size), sf::Color::Blue, shak::Square::Type::Outlined);
    square->SetRotateWithParent(false);
    this->AddChild(square);
}

void Ship::Update(float dt)
{
    this->UpdateDirection();
    this->UpdateLookDirection();
    this->UpdateTextureCoords();
    if (m_distanceToDestination > 10.f)
    {
        const auto offset = m_direction * m_speed * dt;
        this->move(offset);
        m_distanceToDestination -= offset.length();
    }
}

void Ship::HandleInput(const sf::Event& event)
{
}

void Ship::OnCollision(const std::shared_ptr<GameObject>& other)
{
    // if (this->Name == "Player")
        // std::cout << "Ship collided with " << other->Name << std::endl;
}

void Ship::SetTarget(const std::shared_ptr<Ship>& target)
{
    if (m_target)
    {
        if (target->Id == m_target->Id)
            return;
        else
        {
            m_target->ToggleAimSprite(false);
            OnAutoAttackStopped();
        }
    }

    m_target = target;
    OnTargetChanged();
    m_target->ToggleAimSprite(true);
    m_targetWasSelected = true;
}

bool Ship::IsCritHit() const
{
    float critChance = m_shipData.at("base_stats").at("crit").ToFloat();
    return (static_cast<double>(std::rand()) / RAND_MAX) <= critChance;
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
    // for (auto& laser : m_lasers)
        // laser->rotateAround(lasersAngle, this->getPosition());
}

void Ship::UpdateTextureCoords()
{
    auto coords = m_atlas->GetTextureCoords(GetTextureByDirection());
    (*m_vertices)[0].texCoords = coords.topLeft;
    (*m_vertices)[1].texCoords = coords.bottomLeft;
    (*m_vertices)[2].texCoords = coords.topRight;
    (*m_vertices)[3].texCoords = coords.bottomRight;
}

void Ship::ToggleAimSprite(bool show)
{
    m_aimSprite->SetTransparency(show ? 255 : 0);
}

// Events
void Ship::SpawnDamageNumber(float damage)
{
    auto damageNumber = m_damageNumberPool.Get();
    damageNumber->Reset(static_cast<int>(damage), this->getPosition(), sf::Color::Red, 10);
    m_engine->GetScene()->AddGameObject(damageNumber);
}

void Ship::SpawnCritDamageNumber(float damage)
{
    auto damageNumber = m_damageNumberPool.Get();
    damageNumber->Reset(static_cast<int>(damage), this->getPosition(), sf::Color::Yellow, 15);
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

