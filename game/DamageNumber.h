#include "ShakEngine.h"

class DamageNumber : public shak::GameObject
{
public:
    DamageNumber(int damage, sf::Vector2f position, sf::Vector2f velocity)
        : m_damage(damage), m_text(m_font), m_position(position), m_velocity(velocity), m_lifeTime(1.f), m_elapsedTime(0.f), m_isAlive(true)
    {
        m_font = sf::Font("assets/fonts/Roboto/static/Roboto-Black.ttf");
        m_text.setFont(m_font);
        m_text.setString(std::to_string(m_damage));
        m_text.setCharacterSize(20);
        m_text.setFillColor(sf::Color::Red);
        m_text.setPosition(m_position);
    }
    ~DamageNumber() {}

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        if (!m_isAlive) return;

        states.transform *= this->getTransform();
        target.draw(m_text, states);
    }

    void Update(float dt) override
    {
        GameObject::Update(dt);

        if (m_isAlive)
        {
            m_elapsedTime += dt;
            m_text.move(m_velocity * dt);

            if (m_elapsedTime >= m_lifeTime)
                m_isAlive = false;
        }
    }

    void Reset()
    {
        m_isAlive = true;
        m_elapsedTime = 0.f;
    }

private:
    int m_damage;
    sf::Text m_text;
    sf::Font m_font;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_lifeTime;
    float m_elapsedTime;
    bool m_isAlive;
};

