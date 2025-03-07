#include "ShakEngine.h"
#include "defaultScenes/EngineLogoSplash.h"

int main()
{
    shak::ShakEngine* engine = &shak::ShakEngine::GetInstance();
    engine->Initialize();
    auto sm = engine->GetSceneManager();
    engine->GetResourceManager()->SetPrefixPath("../..");
    sm->AddScene("test", std::make_shared<EngineLogoSplash>());
    sm->ActivateScene("test");
    engine->Start();
}