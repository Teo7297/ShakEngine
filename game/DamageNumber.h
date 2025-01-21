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
        float t = m_elapsedTime / m_lifeTime;

        // Movement
        {
            // Get animation index
            int posCurveIndex = t * (m_posVelocityCurve.size() - 1) + 1;
            posCurveIndex = std::min(posCurveIndex, static_cast<int>(m_posVelocityCurve.size() - 1));

            // Compute position
            float posCurveVelocity = std::lerp(m_posVelocityCurve[posCurveIndex - 1], m_posVelocityCurve[posCurveIndex], t);
            m_text.move(m_dir * posCurveVelocity * m_speed * dt);
        }

        // Scaling
        {
            // Get animation index
            float sizeCurveIndexf = t * (float)(m_sizeVelocityCurve.size() - 1) + 1.f;
            int sizeCurveIndex = static_cast<int>(sizeCurveIndexf);
            float tf = sizeCurveIndexf - sizeCurveIndex;
            sizeCurveIndex = std::min(sizeCurveIndex, static_cast<int>(m_sizeVelocityCurve.size() - 1));

            // Compute scale
            float sizeCurveVelocity = std::lerp(m_sizeVelocityCurve[sizeCurveIndex - 1], m_sizeVelocityCurve[sizeCurveIndex], tf) + 1.f;
            m_text.setScale({ sizeCurveVelocity, sizeCurveVelocity });
        }

        // Disable if lifetime is over
        if (m_elapsedTime >= m_lifeTime)
            this->SetActive(false);
    }

    void Reset(int damage, sf::Vector2f position)
    {
        this->setPosition({ 0.f, 0.f }); // Needed because we update the text position not the GameObject position. Hence if this gets deactivated and reactivated, the text will be in the wrong position because the GameObject position does not get updated while inactive.
        m_text.setOrigin({ m_text.getLocalBounds().size.x / 2.f, m_text.getLocalBounds().size.y / 2.f });
        m_text.setPosition(position);
        m_text.setScale({ 1.f, 1.f });
        m_text.setString(std::to_string(damage));
        m_text.setCharacterSize(15);
        m_text.setFillColor(sf::Color::Red);
        m_text.setOutlineThickness(1.f);
        m_text.setOutlineColor(sf::Color::Black);

        // random direction
        m_dir = sf::Vector2f{ static_cast<float>(rand() % 100 - 50), static_cast<float>(rand() % 100 - 50) };
        // 0,0 is not a valid direction
        if(m_dir == sf::Vector2f{ 0.f, 0.f })
            m_dir = sf::Vector2f{ 1.f, 1.f };
        m_dir = m_dir.normalized();
        this->move(m_dir * 100.f);

        this->SetActive(true);
        m_elapsedTime = 0.f;
        m_lifeTime = 1.f;
    }

private:
    sf::Text m_text;
    sf::Font m_font;
    float m_lifeTime;
    float m_elapsedTime;
    float m_speed = 100.f;

    sf::Vector2f m_dir;
    // Curve for the damage number to follow
    std::vector<float> m_sizeVelocityCurve = { 10.f, 1.f, 2.f, 1.4f, 1.3f, 1.2f, 1.1f, 1.f };
    std::vector<float> m_posVelocityCurve = { 0.f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.f };
};

