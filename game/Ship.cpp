#include "Ship.h"
#include "components/Health.h"

#define _USE_MATH_DEFINES
#include <math.h>

Ship::Ship(const std::shared_ptr<shak::TextureAtlas> atlas, const std::vector<sf::Vector2f> lasersOffsets, const std::shared_ptr<shak::TextureAtlas> deathAnimation)
    : GameObject(),
    m_atlas(atlas),
    m_atlasTexturesCount{ atlas->GetCount() },
    m_direction{ 1.f, 0.f }, // spawn looking right
    m_lookDirection{ 1.f, 0.f },
    m_destination{ 0.f, 0.f },
    m_damageNumberPool(20),
    m_laserShotPool(20),
    m_target{ nullptr },
    m_lasers{ },
    m_laserOffsets{ lasersOffsets },
    m_laserIndex{ 0 },
    m_speed{ 1000.f },
    m_hp{ 20000.f },
    m_maxHp{ 20000.f },
    m_damage{ 1000.f },
    m_shield{ 100000.f },
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

    auto origin = m_vertices->getBounds().getCenter();
    this->setOrigin(origin);

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

void Ship::HandleInput(const sf::Event& event)
{
    GameObject::HandleInput(event);
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
    health->OnDamage.Add([this](float damage)
        {
            auto info = this->TakeDamage(damage);
        });

    GameObject::Awake();
}

void Ship::Update(float dt)
{
    if ((getPosition() - m_destination).lengthSquared() > 10.f)
        this->move(m_direction * m_speed * dt);

    GameObject::Update(dt);
}

LaserShot::HitInfo Ship::TakeDamage(float damage)
{
    m_hp -= damage;

    auto damageNumber = m_damageNumberPool.Get();
    damageNumber->Reset(static_cast<int>(damage), this->getPosition());
    m_engine->AddGameObject(damageNumber);

    LaserShot::HitInfo info{
        .damage = damage,
        .killed = m_hp <= 0.f,
    };

    if (m_hp <= 0.f)
    {
        ToggleAimSprite(false);
        m_deathAnimation->setPosition(this->getPosition()); // make sure it's in the right place
        m_deathAnimation->Play();
        // this->move({ 3000.f, 3000.f }); // move out of the way
        m_hp = m_maxHp;
    }
    return info;
}

int Ship::GetTextureByDirection() const
{
    const float directionAngle = m_lookDirection.angleTo({ 1.f, 0.f }).asRadians(); // Distance from right dir (aka 0 degrees)

    float signedAngle = directionAngle / (2.0f * static_cast<float>(M_PI));
    if (signedAngle < 0.f)
        signedAngle += 1.f;
    return static_cast<int>(signedAngle * static_cast<float>(m_atlasTexturesCount)) % m_atlasTexturesCount;
}

void Ship::UpdateDirection()
{
    const auto dir = m_destination - getPosition();
    if (dir.lengthSquared() > 0.f)
        m_direction = dir.normalized();
}

void Ship::UpdateLookDirection()
{
    auto prevLookDirection = m_lookDirection;
    if (m_lookAtTarget && m_target)
        m_lookDirection = m_target->getPosition() - getPosition();
    else
        m_lookDirection = m_direction;

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

LaserShot::HitInfo Ship::OnLaserHit()
{
    if (!m_target)
        return
    {
        .damage = 0.f,
        .killed = false
    };

    // auto info = m_target->TakeDamage(m_damage + std::rand() % 10000);
    // if (info.killed)
    // {
    //     this->SetTarget(nullptr);
    // }

    auto health = m_target->GetComponent<Health>();
    if (health)
    {
        health->TakeDamage(m_damage + std::rand() % 10000);
    }

    return {
        .damage = 0.f,
        .killed = false
    };
}

