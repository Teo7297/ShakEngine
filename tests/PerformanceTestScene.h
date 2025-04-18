#pragma once

#include "Scene.h"

#include "Camera.h"
#include "shapes/Square.h"
#include "PerformanceTestUI.h"

class MovingSquare : public shak::Square
{
public:
    MovingSquare(const sf::FloatRect& square) : Square(square, sf::Color::Red, shak::Square::Type::Filled)
    {
        this->setOrigin(square.size / 2.f);
    }
    ~MovingSquare() override = default;
    void Update(float dt) override
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            this->move({ 0.f, -100.f * dt });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            this->move({ 0.f, 100.f * dt });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            this->move({ -100.f * dt, 0.f });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            this->move({ 100.f * dt, 0.f });
        }
    }
};

class PerformanceTestScene : public shak::Scene
{
public:
    PerformanceTestScene(const std::string& name) : Scene(name)
    {
        std::cout << "Test scene created" << std::endl;
    }
    ~PerformanceTestScene() override
    {
        std::cout << "Test scene destroyed" << std::endl;
    }

    void Init() override
    {
        auto camera1 = m_renderer->AddCamera("camera1", sf::FloatRect({ 0, 0 }, { 1920, 1080 }));


        for (int i = 0; i < 10000; i++)
        {
            // Create a square in random position
            sf::FloatRect squareRect = sf::FloatRect(
                {
                    static_cast<float>(rand() % 1920),
                    static_cast<float>(rand() % 1080)
                },
                { 5.f, 5.f }
            );
            auto square = m_engine->AddGameObject<MovingSquare>(squareRect);
            square->Name = "Square" + std::to_string(i);
        }

        m_engine->AddUIElement<PerformanceTestUI>("ui");
        m_engine->SelectActiveUI("ui");

    }

    void Clear() override
    {
        std::cout << "Test scene clear called" << std::endl;
    }
};