#include "ShakEngine.h"
#include "TestScene.h"

int main()
{
    shak::ShakEngine* engine = &shak::ShakEngine::GetInstance();
    engine->Initialize();
    auto sm = engine->GetSceneManager();
    sm->AddScene(std::make_shared<TestScene>("test"));
    sm->QueueScene("test");
    sm->TryActivateQueuedScene();
    engine->Start();
}