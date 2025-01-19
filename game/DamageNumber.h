#pragma once

#include "ShakEngine.h"

class DamageNumber : public shak::GameObject
{
public:
    DamageNumber()
        : m_font("assets/fonts/Roboto/static/Roboto-Black.ttf"), m_text(m_font)
    {
    }
    ~DamageNumber() {}

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= this->getTransform();
        target.draw(m_text, states);
    }

    void Update(float dt) override
    {
        GameObject::Update(dt);

        m_elapsedTime += dt;
        m_text.move(m_velocity * dt);

        if (m_elapsedTime >= m_lifeTime)
            this->SetActive(false);
    }

    void Reset(int damage, sf::Vector2f position, sf::Vector2f velocity)
    {
        this->setPosition({ 0.f, 0.f }); // Needed because we update the text position not the GameObject position. Hence if this gets deactivated and reactivated, the text will be in the wrong position because the GameObject position does not get updated while inactive.
        m_text.setPosition(position);
        m_text.setString(std::to_string(damage));
        m_text.setCharacterSize(20);
        m_text.setFillColor(sf::Color::Red);
        m_velocity = velocity;

        this->SetActive(true);
        m_elapsedTime = 0.f;
        m_lifeTime = 1.f;
    }

private:
    sf::Text m_text;
    sf::Font m_font;
    sf::Vector2f m_velocity;
    float m_lifeTime;
    float m_elapsedTime;
};

