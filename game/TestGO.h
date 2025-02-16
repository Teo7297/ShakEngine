#pragma once

#include "GameObject.h"
#include "ParticleSystem.h"

class TestGO : public shak::GameObject
{
public:
    TestGO()
        : shak::GameObject()
    {
        Name = "TestGO";
    }
    ~TestGO() override = default;

    void Awake() override
    {
        
    }

    void Update(float dt) override {};
    void HandleInput(const sf::Event& event) override
    {
        if (auto key = event.getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::E)
            {
                this->rotate(sf::degrees(5.f));
            }

            if (key->code == sf::Keyboard::Key::Q)
            {
                this->rotate(sf::degrees(-5.f));
            }
        }
    }
    void OnDestroy() override {};
};