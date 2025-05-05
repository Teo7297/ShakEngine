#pragma once

#include "Scene.h"

#include "Camera.h"
#include "shapes/Square.h"

class TestScene : public shak::Scene
{
public:
    TestScene(const std::string& name) : Scene(name)
    {
        std::cout << "Test scene created" << std::endl;
    }
    ~TestScene() override
    {
        std::cout << "Test scene destroyed" << std::endl;
    }

    void Init() override
    {
        auto camera1 = m_renderer->AddCamera("camera1", sf::FloatRect({ 0, 0 }, { 1920, 1080 }), shak::CameraResizeBehavior::Letterbox);

        auto player = m_engine->AddGameObject<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 100.f, 100.f }));
        player->Name = "Player";
        camera1->setPosition(player->getPosition());

        auto child1 = std::make_shared<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 50.f, 50.f }), sf::Color::Red);
        child1->Name = "Child1";
        child1->setPosition(player->getPosition() + sf::Vector2f{ 300.f, 0.f });
        player->AddChild(child1);

        auto child2 = std::make_shared<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 50.f, 50.f }), sf::Color::Red);
        child2->Name = "Child2";
        child2->setPosition(player->getPosition() + sf::Vector2f{ -300.f, 0.f });
        player->AddChild(child2);

        auto child3 = std::make_shared<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 50.f, 50.f }), sf::Color::Red);
        child3->Name = "Child3";
        child3->setPosition(player->getPosition() + sf::Vector2f{ 0.f, 300.f });
        player->AddChild(child3);

        auto child4 = std::make_shared<shak::Square>(sf::FloatRect({ 0.f,0.f }, { 50.f, 50.f }), sf::Color::Red);
        child4->Name = "Child4";
        child4->setPosition(player->getPosition() + sf::Vector2f{ 0.f, -300.f });
        player->AddChild(child4);

        auto rm = m_engine->GetResourceManager();
        auto shadertest = rm->LoadShader("", "IDONTCARE/particle.fs", "testshader");
        if (shadertest)
            std::cout << "Shader loaded from cache" << std::endl;
        else
        {
            std::cerr << "Shader NOT loaded from cache" << std::endl;
            exit(-1);
        }
        player->SetShader(shadertest);

        shadertest = rm->LoadShader("", "../../tests/shadertest/test.fs", "testshader");
        if (shadertest)
            std::cout << "Shader loaded from text file" << std::endl;
        else
        {
            std::cerr << "Shader NOT loaded from text file" << std::endl;
            exit(-1);
        }
        player->SetShader(shadertest);

        Clear();

        m_engine->GetRenderer()->CloseWindow();
    }

    void Clear() override
    {
        std::cout << "Test scene clear called" << std::endl;
    }
};