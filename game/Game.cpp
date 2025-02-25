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

void game()
{
    shak::ShakEngine* engine = &shak::ShakEngine::GetInstance();
    engine->Initialize();
    auto sm = engine->GetSceneManager();
    sm->AddScene("test", std::make_shared<TestGameScene>());
    sm->AddScene("test2", std::make_shared<TestGameScene2>());
    sm->ActivateScene("test");
    engine->Start();
}


int main()
{
    game();
    return 0;
}
