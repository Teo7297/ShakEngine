#include "PlayerController.h"
#include "Ship.h"

void PlayerController::Awake()
{
    m_owner->Name = "Player";
    m_engine = &shak::ShakEngine::GetInstance();
    m_ownerShip = (Ship*)m_owner;
}

void PlayerController::Update(float dt)
{
    if (!m_ownerShip->GetTargetWasSelected()) // we can move
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            // Move if click outside of ship
            auto mousePos = m_engine->GetMouseWorldPos();
            bool updateDestination = !m_ownerShip->IsPointInside(mousePos);

            // Click valid only if inside window
            auto windowSize = m_engine->GetWindowSize();
            auto mousePosPixel = m_engine->GetMousePixelPos();
            updateDestination &= mousePosPixel.x >= 0 && mousePosPixel.x <= windowSize.x;
            updateDestination &= mousePosPixel.y >= 0 && mousePosPixel.y <= windowSize.y;

            if (updateDestination)
            {
                m_ownerShip->SetDestination(mousePos);
            }
        }
    }
}

void PlayerController::HandleInput(const sf::Event& event)
{
    if (auto key = event.getIf<sf::Event::KeyPressed>())
    {

    }

    else if (auto key = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (key->button == sf::Mouse::Button::Left)
        {
            auto vec = m_engine->FindGameObjectsByType<Ship>();
            for (auto& ship : vec)
            {
                if (ship->Name == "Player")
                    continue;
                else if (ship->IsPointInside(m_engine->GetMouseWorldPos()))
                {
                    auto newTarget = std::dynamic_pointer_cast<Ship>(ship);
                    m_ownerShip->SetTarget(newTarget);
                    break;
                }
            }
        }
    }
    else if (auto key = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (key->button == sf::Mouse::Button::Left)
        {
            m_ownerShip->ResetTargetWasSelected();
        }
    }
}
