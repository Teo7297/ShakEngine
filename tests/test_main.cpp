#include "ShakEngine.h"
#include "TestScene.h"

int main()
{
    shak::ShakEngine* engine = &shak::ShakEngine::GetInstance();
    engine->Initialize();
    auto sm = engine->GetSceneManager();
    sm->AddScene("test", std::make_shared<TestScene>());
    sm->QueueScene("test");
    sm->TryActivateQueuedScene();
    engine->Start();
}