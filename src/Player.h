#pragma once

#include "ShakEngine.h"

class Player : public shak::GameObject
{
public:
    Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<sf::VertexArray> va, std::shared_ptr<sf::Texture> texture = nullptr)
        : GameObject(va, texture), m_engine(engine)
    {
    }
    ~Player() = default;

    void HandleInput(const sf::Event& event) override
    {
        GameObject::HandleInput(event);
    }

    void Update(float dt) override
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            this->move({ 0, -m_speed * dt });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            this->move({ 0, m_speed * dt });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            this->move({ -m_speed * dt, 0 });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            this->move({ m_speed * dt, 0 });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
            this->rotate(sf::degrees(-200 * dt));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
            this->rotate(sf::degrees(200 * dt));
        
        // m_shader->setUniform("mouse", m_engine->GetMousePixelPos());

        GameObject::Update(dt);
    }

private:
    float m_speed = 1000.f;
    sf::Vector2f m_direction;
    std::shared_ptr<ShakEngine> m_engine;
};
