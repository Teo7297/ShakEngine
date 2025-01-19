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

        auto coords = goliathPlus->GetTextureCoords(1);
        auto quad = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
        (*quad)[0].position = sf::Vector2f(0.f, 0.f);
        (*quad)[1].position = sf::Vector2f(0.f, 250.f);
        (*quad)[2].position = sf::Vector2f(250.f, 0.f);
        (*quad)[3].position = sf::Vector2f(250.f, 250.f);
        (*quad)[0].color = sf::Color::White;
        (*quad)[1].color = sf::Color::White;
        (*quad)[2].color = sf::Color::White;
        (*quad)[3].color = sf::Color::White;
        (*quad)[0].texCoords = coords.topLeft;
        (*quad)[1].texCoords = coords.bottomLeft;
        (*quad)[2].texCoords = coords.topRight;
        (*quad)[3].texCoords = coords.bottomRight;

        auto player = std::make_shared<Player>(engine, quad, goliathPlus);
        player->move({ 1920 / 2, 1080 / 2 });
        player->AddChild(camera1);
        auto bg = std::make_shared<shak::Background>(rm.LoadTexture("assets/textures/bg1.jpg", "bg1", true), sf::Vector2f(1920.f, 1080.f));
        camera1->SetBackground(bg);

        engine->AddGameObject(bg);

        engine->AddGameObject(player);

        // camera1->move({ -1920 / 2, -1080 / 2 });

        auto bricks = rm.LoadTexture("assets/textures/Bricks.jpg", "bricks");
        auto ps = std::make_shared<shak::ParticleSystem>(500, 0.f, 2.f, 3.f, 2.f, 15.f, sf::Vector2(-200.f, -200.f), sf::Vector2f(200.f, 200.f), sf::Color::Blue, sf::Color::Green, bricks);

        ps->setPosition(player->getPosition());
        player->AddChild(ps);
    }

    engine->Start();
    return 0;
}

// #include "StripTrailExample.h"

// int main()
// {
//     StripTrailExample();
// }

// #include "TextureAtlasExample.h"
// int main()
// {
//     TextureAtlasExample();
// }

