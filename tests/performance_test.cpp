#include "ShakEngine.h"
#include "PerformanceTestScene.h"

int main()
{
    shak::ShakEngine* engine = &shak::ShakEngine::GetInstance();
    engine->Initialize();
    auto sm = engine->GetSceneManager();
    sm->AddScene(std::make_shared<PerformanceTestScene>("test"));
    sm->QueueScene("test");
    sm->TryActivateQueuedScene();
    engine->Start();
}