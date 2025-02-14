#pragma once

#include "ShakSprite.h"
#include "Spline.h"
#include "Player.h"

class Missle : public shak::Sprite
{
public:
    Missle(const std::shared_ptr<sf::Texture>& texture, const std::shared_ptr<Player>& player)
        : Sprite(texture), m_t(0.f), m_previousPos(this->getPosition()), m_player(player)
    {
        m_start = { 0.f, 0.f };
        m_destination = { 500.f, 500.f };

        m_spline = { m_start, { 0.f, -300.f }, { 250.f, 400.f }, m_destination };
    }

    void Update(float dt) override
    {
        // Update moving destination
        auto newDest = m_player->getPosition();
        m_spline = { m_start, { 0.f, -300.f }, { 250.f, 400.f }, newDest };


        m_t += dt;
        if (m_t > 1.f)
            m_t = 0.f;

        auto pos = m_spline.GetPoint(m_t);

        auto dir = (pos - m_previousPos).normalized();
        auto angle = dir.angle();
        angle += sf::degrees(90.f);
        this->setRotation(angle);

        m_previousPos = pos;
        this->setPosition(pos);
    }


private:
    sf::Vector2f m_start, m_destination;
    shak::Spline m_spline;

    sf::Vector2f m_previousPos;

    // test
    std::shared_ptr<Player> m_player;


    float m_t;
};