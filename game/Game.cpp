#include "ShakEngine.h"

#include "ParticleSystem.h"
#include "Background.h"

#include "Animation.h"
#include "Spline.h"
#include "Rocket.h"
#include "components/PlayerController.h"
#include "components/AIController.h"

#include "shapes/Square.h"
#include "shapes/Circle.h"

#include "ShaderDevHelper.h"
#include "TestGO.h"
#include "UI/HUD.h"

#include "SceneManager.h"
#include "scenes/TestGameScene.h"
#include "scenes/TestGameScene2.h"


void ShaderTest()
{
    shak::ShakEngine* engine = &shak::ShakEngine::GetInstance();
    engine->Initialize();
    {
        auto& rm = engine->GetResourceManager();

        auto camera1 = std::make_shared<shak::Camera>(sf::FloatRect({ 0, 0 }, { 1920, 1080 }));
        // engine->AddCamera("camera1", camera1);
        camera1->move({ 1920.f / 2.f, 1080.f / 2.f });

        // auto testTexture = rm.LoadTexture("assets/textures/abstract1.png", "testTexture", true, true);
        // auto testTexture = rm.LoadTexture("assets/textures/rocket.png", "testTexture", false, true);
        engine->AddGameObject<ShaderDevHelper>("healthbar", nullptr);
    }

    engine->Start();
}

void game()
{
    shak::ShakEngine* engine = &shak::ShakEngine::GetInstance();
    engine->Initialize();
    auto sm = engine->GetSceneManager();
    sm->AddScene("test", std::make_shared<TestGameScene>());
    sm->AddScene("test2", std::make_shared<TestGameScene2>());
    sm->ActivateScene("test2");
    engine->Start();
}


int main()
{
    game();
    // ShaderTest();
    return 0;
}
