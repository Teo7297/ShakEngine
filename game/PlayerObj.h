#pragma once

#include "ShakEngine.h"
#include "Ship.h"

class PlayerObj : public shak::GameObject
{
public:
    void Awake() override
    {
        Name = "Player";
    }

    void Update(float dt) override
    {
        if (!m_activeShip->GetTargetWasSelected()) // we can move
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                // Move if click outside of ship
                auto mousePos = m_engine->GetMouseWorldPos();
                bool updateDestination = !m_activeShip->IsPointInside(mousePos);

                // Click valid only if inside window
                auto windowSize = m_engine->GetWindowSize();
                auto mousePosPixel = m_engine->GetMousePixelPos();
                updateDestination &= mousePosPixel.x >= 0 && mousePosPixel.x <= windowSize.x;
                updateDestination &= mousePosPixel.y >= 0 && mousePosPixel.y <= windowSize.y;

                if (updateDestination)
                {
                    m_activeShip->SetDestination(mousePos);
                }
            }
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override {}

    void HandleInput(const sf::Event& event) override
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
                        m_activeShip->SetTarget(newTarget);
                        break;
                    }
                }
            }
        }
        else if (auto key = event.getIf<sf::Event::MouseButtonReleased>())
        {
            if (key->button == sf::Mouse::Button::Left)
            {
                m_activeShip->ResetTargetWasSelected();
            }
        }
    }

    void SetShip(const std::shared_ptr<Ship>& ship)
    {
        if (m_activeShip)
        {
            m_engine->Destroy(m_activeShip);
            m_activeShip = nullptr;
        }

        m_activeShip = ship;
        m_activeShip->SetControlledByPlayer(true);
        this->AddChild(m_activeShip);
    }

    std::shared_ptr<Ship> GetShip() const { return m_activeShip; }

private:
    std::shared_ptr<Ship> m_activeShip;

};