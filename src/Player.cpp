#include "Player.h"
#include "DamageNumber.h"


Player::Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<sf::VertexArray> va, std::shared_ptr<shak::TextureAtlas> atlas)
    : GameObject(va, atlas->GetAtlasTexture()), m_engine(engine), m_atlas(atlas), m_atlasTexturesCount(atlas->GetCount())
{
    m_bbSize = va->getBounds().size;
    this->setOrigin(va->getBounds().getCenter());
}

void Player::HandleInput(const sf::Event& event)
{
    if(auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if(key->code == sf::Keyboard::Key::Space)
        {
            auto damageNumber = std::make_shared<DamageNumber>(100, getPosition(), sf::Vector2f(0.f, -100.f));
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
        auto mousePosePixel = m_engine->GetMousePixelPos();
        updateDestination &= mousePosePixel.x >= 0 && mousePosePixel.x <= windowSize.x;
        updateDestination &= mousePosePixel.y >= 0 && mousePosePixel.y <= windowSize.y;

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
    const auto directionAngle = m_direction.angleTo({ 1.f, 0.f }); // Distance from right dir (aka 0 degrees)
    float signedAngle = (directionAngle.asRadians() / (2.f * 3.14159265f));
    if (signedAngle < 0.f)
        signedAngle += 1.f;
    return static_cast<int>(signedAngle * m_atlasTexturesCount) % m_atlasTexturesCount;
}
