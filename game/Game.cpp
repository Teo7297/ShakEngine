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


        auto laserTxt = rm.LoadTexture("assets/textures/abstract1.png", "laser", true, true);
        auto laserSh = rm.LoadShader("", "assets/shaders/laserShot.fs", "laserShot");
        laserSh->setUniform("u_texture", *laserTxt);
        laserSh->setUniform("u_resolution", sf::Glsl::Vec2{ engine->GetWindowSize().x, engine->GetWindowSize().y });

        auto explosionAtlas = rm.LoadTextureAtlas("assets/animations/explosion.atlas", "deathExplosion");

        // auto bg = engine->AddGameObject<shak::Background>(rm.LoadTexture("assets/textures/bg1.jpg", "bg1", true), sf::Vector2f(1920.f, 1080.f));
        // camera1->SetBackground(bg);
        // auto bgsize = rm.GetTexture("bg1")->getSize();
        // camera1->SetBackgroundSize({ (float)(bgsize.x * 100), (float)(bgsize.y * 100) });


        std::ifstream file("assets/json/ships/DPS.json");
        if (!file.is_open()) {
            std::cerr << "Failed to open file.\n";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        json::JSON jsonData = json::JSON::Load(content);

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

        auto rocketTxt = rm.LoadTexture("assets/textures/rocket.png", "rocket");
        auto rocketShd = rm.LoadShader("", "assets/shaders/rocket.fs", "rocket");
        auto rocket = engine->AddGameObject<Rocket>(rocketTxt, rocketShd);
        rocket->Init({ 0.f, 0.f }, player);
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

#include "JSON.h"

int jsontest() {
    std::ifstream file("assets/json/ships/DPS.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    json::JSON jsonData = json::JSON::Load(content);
    std::cout << jsonData.dump() << std::endl;

    return 0;
}

int main()
{
    game();
    // ShaderTest();
    // jsontest();

    return 0;
}
