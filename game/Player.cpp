#include "Player.h"
#include "DamageNumber.h"

#define _USE_MATH_DEFINES
#include <math.h>

Player::Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<sf::VertexArray> va, std::shared_ptr<shak::TextureAtlas> atlas)
    : GameObject(va, atlas->GetAtlasTexture()), m_engine(engine), m_atlas(atlas), m_atlasTexturesCount(atlas->GetCount())
{
    m_bbSize = va->getBounds().size;
    this->setOrigin(va->getBounds().getCenter());
}

void Player::HandleInput(const sf::Event& event)
{
    if (auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Space)
        {
            auto damageNumber = m_damageNumberPool.Get();
            damageNumber->Reset(100, this->getPosition(), { 0.f, -100.f });
            this->AddChild(damageNumber);
        }
    }
    GameObject::HandleInput(event);
}

void Player::Update(float dt)
{
    static int c = 0;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        // Move if click outside of ship
        auto mousePos = m_engine->GetMouseWorldPos();
        auto shipPos = getPosition();
        bool updateDestination = !this->IsPointInside(mousePos - shipPos + m_bbSize / 2.f);

        // Click valid only if inside window
        auto windowSize = m_engine->GetWindowSize();
        auto mousePosPixel = m_engine->GetMousePixelPos();
        updateDestination &= mousePosPixel.x >= 0 && mousePosPixel.x <= windowSize.x;
        updateDestination &= mousePosPixel.y >= 0 && mousePosPixel.y <= windowSize.y;

        if (updateDestination)
            m_destination = mousePos;
    }

    if ((getPosition() - m_destination).lengthSquared() > 10.f)
    {
        // Move
        m_direction = m_destination - getPosition();
        m_direction = m_direction.normalized();
        this->move(m_direction * m_speed * dt);

        // Update texture
        auto coords = m_atlas->GetTextureCoords(GetTextureByDirection());
        (*m_vertices)[0].texCoords = coords.topLeft;
        (*m_vertices)[1].texCoords = coords.bottomLeft;
        (*m_vertices)[2].texCoords = coords.topRight;
        (*m_vertices)[3].texCoords = coords.bottomRight;

    }

    GameObject::Update(dt);
}

int Player::GetTextureByDirection() const
{
    const float directionAngle = m_direction.angleTo({ 1.f, 0.f }).asRadians(); // Distance from right dir (aka 0 degrees)
    float signedAngle = directionAngle / (2.0f * M_PI);
    if (signedAngle < 0.f)
        signedAngle += 1.f;
    return static_cast<int>(signedAngle * m_atlasTexturesCount) % m_atlasTexturesCount;
}
