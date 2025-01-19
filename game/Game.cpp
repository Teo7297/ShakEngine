#include "ShakEngine.h"

#include "Player.h"

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

        engine->AddGameObject(player);

        camera1->move({ -1920 / 2, -1080 / 2 });

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

