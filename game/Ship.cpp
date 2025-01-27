#include "Ship.h"

#define _USE_MATH_DEFINES
#include <math.h>

Ship::Ship(const std::shared_ptr<shak::TextureAtlas> atlas)
    : GameObject(), m_atlas(atlas), m_atlasTexturesCount{ atlas->GetCount() }
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

    this->setOrigin(m_vertices->getBounds().getCenter());
}

void Ship::HandleInput(const sf::Event& event)
{
    GameObject::HandleInput(event);
}

void Ship::Awake()
{
    GameObject::Awake();
}

void Ship::Update(float dt)
{
    if ((getPosition() - m_destination).lengthSquared() > 10.f)
        this->move(m_direction * m_speed * dt);

    GameObject::Update(dt);
}

float Ship::TakeDamage(float damage)
{
    m_hp -= damage;

    auto damageNumber = m_damageNumberPool.Get();
    damageNumber->Reset(damage, this->getPosition());
    this->AddChild(damageNumber);

    if (m_hp <= 0.f)
    {
        std::cout << "Ship destroyed!" << std::endl;
        m_hp = m_maxHp;
    }
    return damage;
}

int Ship::GetTextureByDirection() const
{
    sf::Vector2f direction;
    if (m_lookAtTarget && m_target)
        direction = m_target->getPosition() - getPosition();
    else
        direction = m_direction;

    const float directionAngle = direction.angleTo({ 1.f, 0.f }).asRadians(); // Distance from right dir (aka 0 degrees)

    float signedAngle = directionAngle / (2.0f * M_PI);
    if (signedAngle < 0.f)
        signedAngle += 1.f;
    return static_cast<int>(signedAngle * m_atlasTexturesCount) % m_atlasTexturesCount;
}

void Ship::UpdateDirection()
{
    m_direction = m_destination - getPosition();
    m_direction = m_direction.normalized();
}

void Ship::UpdateTextureCoords()
{
    auto coords = m_atlas->GetTextureCoords(GetTextureByDirection());
    (*m_vertices)[0].texCoords = coords.topLeft;
    (*m_vertices)[1].texCoords = coords.bottomLeft;
    (*m_vertices)[2].texCoords = coords.topRight;
    (*m_vertices)[3].texCoords = coords.bottomRight;
}

void Ship::SetTarget(std::shared_ptr<Ship> target)
{
    m_target = target;
}

