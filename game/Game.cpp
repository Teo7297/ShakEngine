#include "ShakEngine.h"

#include "Player.h"
#include "ParticleSystem.h"
#include "Background.h"

int main()
{
    auto engine = std::make_shared<ShakEngine>();

    {
        auto& rm = engine->GetResourceManager();
        auto goliathPlus = rm.LoadTextureAtlas("assets/textures/New_Goliath_plus/ship106.atlas", "goliathPlus");

        auto camera1 = std::make_shared<shak::Camera>(sf::FloatRect({ 0, 0 }, { 1920, 1080 }));
        engine->AddCamera("camera1", camera1);

        
        auto player = std::make_shared<Player>(engine, goliathPlus);
        player->AddChild(camera1);
        auto bg = std::make_shared<shak::Background>(rm.LoadTexture("assets/textures/bg1.jpg", "bg1", true), sf::Vector2f(1920.f, 1080.f));
        camera1->SetBackground(bg);
        camera1->SetBackgroundSize({ 10000.f, 8000.f });

        engine->AddGameObject(bg);

        engine->AddGameObject(player);

        camera1->move({ -1920 / 2, -1080 / 2 });

        // auto bricks = rm.LoadTexture("assets/textures/Bricks.jpg", "bricks");
        auto chicken = rm.LoadTexture("assets/textures/Portrait.png", "chicken");
        auto ps = std::make_shared<shak::ParticleSystem>(500, 0.f, 2.f, 3.f, 2.f, 15.f, sf::Vector2(-200.f, -200.f), sf::Vector2f(200.f, 200.f), sf::Color::Blue, sf::Color::Green, chicken);

        ps->setPosition(player->getPosition());

        player->AddChild(ps);
    }

    engine->Start();
    return 0;
}
