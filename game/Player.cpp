#include "Player.h"

Player::Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<shak::TextureAtlas> atlas)
    : Ship(atlas), m_engine(engine)
{
}

void Player::HandleInput(const sf::Event& event)
{
    if (auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Space)
        {
            auto damageNumber = m_damageNumberPool.Get();
            damageNumber->Reset(100, this->getPosition());
            this->AddChild(damageNumber);
        }
    }

    Ship::HandleInput(event);
}

void Player::Update(float dt)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        // Move if click outside of ship
        auto mousePos = m_engine->GetMouseWorldPos();
        bool updateDestination = !this->IsPointInside(mousePos);

        // Click valid only if inside window
        auto windowSize = m_engine->GetWindowSize();
        auto mousePosPixel = m_engine->GetMousePixelPos();
        updateDestination &= mousePosPixel.x >= 0 && mousePosPixel.x <= windowSize.x;
        updateDestination &= mousePosPixel.y >= 0 && mousePosPixel.y <= windowSize.y;

        if (updateDestination)
        {
            m_destination = mousePos;
            this->UpdateDirection();
            this->UpdateTextureCoords();
        }
    }

    Ship::Update(dt);
}
