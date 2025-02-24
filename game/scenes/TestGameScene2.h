#pragma once

#include "Scene.h"

#include "Ship.h"
#include "components/PlayerController.h"
#include "components/AIController.h"
#include "UI/HUD.h"
#include "Camera.h"
#include "shapes/Square.h"

class TestGameScene2 : public shak::Scene
{
public:
    ~TestGameScene2() override
    {
        std::cout << "Test2 scene destroyed" << std::endl;
    }

    void Init() override
    {
        auto camera1 = m_renderer->AddCamera("camera1", sf::FloatRect({ 0, 0 }, { 1920, 1080 }));

        auto player = m_engine->AddGameObject<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 100.f, 100.f }));
        player->Name = "Player";
        player->AddComponent<TestGO>();

        camera1->setPosition(player->getPosition());

        // return;

        auto child1 = std::make_shared<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 50.f, 50.f }), sf::Color::Red);
        child1->Name = "Child1";
        child1->setPosition(player->getPosition() + sf::Vector2f{ 300.f, 0.f });
        player->AddChild(child1);

        auto child2 = std::make_shared<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 50.f, 50.f }), sf::Color::Red);
        child2->Name = "Child2";
        child2->setPosition(player->getPosition() + sf::Vector2f{ -300.f, 0.f });
        player->AddChild(child2);

        return;

        auto child3 = std::make_shared<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 50.f, 50.f }), sf::Color::Red);
        child3->Name = "Child3";
        child3->setPosition(player->getPosition() + sf::Vector2f{ 0.f, 300.f });
        player->AddChild(child3);

        auto child4 = std::make_shared<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 50.f, 50.f }), sf::Color::Red);
        child4->Name = "Child4";
        child4->setPosition(player->getPosition() + sf::Vector2f{ 0.f, -300.f });
        player->AddChild(child4);
    }

    void Clear() override
    {
        std::cout << "Test2 scene clear called" << std::endl;
    }
};