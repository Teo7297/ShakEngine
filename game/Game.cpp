#include "ShakEngine.h"

#include "ParticleSystem.h"
#include "Background.h"

#include "Animation.h"
#include "Spline.h"
#include "Rocket.h"
#include "components/PlayerController.h"
#include "components/AIController.h"

#include "ShaderDevHelper.h"
#include "TestGO.h"

void game()
{
    shak::ShakEngine* engine = &shak::ShakEngine::GetInstance();
    engine->Initialize();
    {
        auto& rm = engine->GetResourceManager();
        auto goliathPlus = rm.LoadTextureAtlas("assets/textures/New_Goliath_plus/ship106.atlas", "goliathPlus");

        auto camera1 = std::make_shared<shak::Camera>(sf::FloatRect({ 0, 0 }, { 1920, 1080 }));

        engine->AddCamera("camera1", camera1);

        auto bg = engine->AddGameObject<shak::Background>(rm.LoadTexture("assets/textures/bg1.jpg", "bg1", true), sf::Vector2f(1920.f, 1080.f));
        camera1->SetBackground(bg);
        auto bgsize = rm.GetTexture("bg1")->getSize();
        camera1->SetBackgroundSize({ (float)(bgsize.x * 100), (float)(bgsize.y * 100) });

        json::JSON jsonData = json::JSON::LoadFromDisk("assets/json/ships/DPS.json");

        auto player = engine->AddGameObject<Ship>(jsonData);
        player->AddComponent<PlayerController>();
        player->AddChild(camera1);

        camera1->move(player->GetVertexArray()->getBounds().size / 2.f);


        for (int i = 0; i < 3; i++)
        {
            auto alien = engine->AddGameObject<Ship>(jsonData);
            alien->AddComponent<AIController>();
            alien->Name = "Alien" + std::to_string(i);
        }
    }

    engine->Start();
}

void ShaderTest()
{
    shak::ShakEngine* engine = &shak::ShakEngine::GetInstance();
    engine->Initialize();
    {
        auto& rm = engine->GetResourceManager();

        auto camera1 = std::make_shared<shak::Camera>(sf::FloatRect({ 0, 0 }, { 1920, 1080 }));
        engine->AddCamera("camera1", camera1);
        camera1->move({ 1920.f / 2.f, 1080.f / 2.f });

        // auto testTexture = rm.LoadTexture("assets/textures/abstract1.png", "testTexture", true, true);
        auto testTexture = rm.LoadTexture("assets/textures/rocket.png", "testTexture", false, true);
        engine->AddGameObject<ShaderDevHelper>("rocket", testTexture);
    }

    engine->Start();
}


int main()
{
    game();
    // ShaderTest();

    return 0;
}
