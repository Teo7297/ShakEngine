#pragma once

#include "Scene.h"

#include "Ship.h"
#include "components/PlayerController.h"
#include "components/AIController.h"
#include "UI/HUD.h"
#include "Camera.h"

class TestGameScene : public shak::Scene
{
public:
    ~TestGameScene() override
    {
        std::cout << "Test scene destroyed" << std::endl;
    }

    void Init() override
    {

        auto camera1 = m_renderer->AddCamera("camera1", sf::FloatRect({ 0, 0 }, { 1920, 1080 }));

        // auto& rm = m_engine->GetResourceManager();
        // auto bg = m_engine->AddGameObject<shak::Background>(rm.LoadTexture("assets/textures/bg1.jpg", "bg1", true), sf::Vector2f(1920.f, 1080.f));
        // camera1->SetBackground(bg);
        // auto bgsize = rm.GetTexture("bg1")->getSize();
        // camera1->SetBackgroundSize({ (float)(bgsize.x * 100), (float)(bgsize.y * 100) });

        json::JSON jsonData = json::JSON::LoadFromDisk("assets/json/ships/DPS.json");

        auto player = m_engine->AddGameObject<Ship>(jsonData);
        player->Name = "Player";
        player->AddComponent<PlayerController>();
        player->AddChild(camera1);
        camera1->setPosition(player->getPosition());

        for (int i = 0; i < 30; i++)
        {
            auto alien = m_engine->AddGameObject<Ship>(jsonData);
            alien->AddComponent<AIController>();
            alien->Name = "Alien" + std::to_string(i);
        }

        m_engine->AddUIElement<HUD>("HUD");
        m_engine->SelectActiveUI("HUD");
    }

    void Clear() override
    {
        m_renderer->RemoveCamera("camera1");
    }
};