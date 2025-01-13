#include "ShakEngine.h"

#include "Player.h"
#include "Camera.h"
int main()
{
    auto engine = std::make_shared<ShakEngine>();

    {
        auto& rm = engine->GetResourceManager();
        auto bricks = rm.LoadTexture("../../../textures/bricks.jpg", "bricks");

        auto camera1 = std::make_shared<shak::Camera>(sf::FloatRect({ 0, 0 }, { 1920, 1080 }));
        engine->AddCamera("camera1", camera1);

        auto quad = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
        (*quad)[0].position = sf::Vector2f(0.f, 0.f);
        (*quad)[1].position = sf::Vector2f(0.f, 100.f);
        (*quad)[2].position = sf::Vector2f(100.f, 0.f);
        (*quad)[3].position = sf::Vector2f(100.f, 100.f);
        (*quad)[0].color = sf::Color::White;
        (*quad)[1].color = sf::Color::White;
        (*quad)[2].color = sf::Color::White;
        (*quad)[3].color = sf::Color::White;
        (*quad)[0].texCoords = sf::Vector2f(0.f, 0.f);
        (*quad)[1].texCoords = sf::Vector2f(0.f, 4096.f);
        (*quad)[2].texCoords = sf::Vector2f(4096.f, 0.f);
        (*quad)[3].texCoords = sf::Vector2f(4096.f, 4096.f);

        auto player = std::make_shared<Player>(engine, quad, bricks);
        // player->AddChild(camera1);

        engine->AddGameObject(player);

    }

    engine->Start();
    return 0;
}