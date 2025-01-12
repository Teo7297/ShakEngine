#include "ShakEngine.h"

#include "Player.h"
int main()
{
    ShakEngine engine;
    auto rm = engine.GetResourceManager();
    auto bricksTxt = rm.LoadTexture("textures/bricks.jpg", "bricks", true, true);

    auto triangle = std::make_shared<sf::VertexArray>(sf::PrimitiveType::Triangles, 3);
    (*triangle)[0].position = sf::Vector2f(0.f, 0.f);
    (*triangle)[1].position = sf::Vector2f(100.f, 0.f);
    (*triangle)[2].position = sf::Vector2f(100.f, 100.f);
    (*triangle)[0].color = sf::Color::Red;
    (*triangle)[1].color = sf::Color::Green;
    (*triangle)[2].color = sf::Color::Blue;
    (*triangle)[0].texCoords = sf::Vector2f(0.f, 0.f);
    (*triangle)[1].texCoords = sf::Vector2f(4096.f, 0.f);
    (*triangle)[2].texCoords = sf::Vector2f(4096.f, 4096.f);

    auto triangle2 = std::make_shared<sf::VertexArray>(sf::PrimitiveType::Triangles, 3);
    (*triangle2)[0].position = sf::Vector2f(0.f, 0.f);
    (*triangle2)[1].position = sf::Vector2f(100.f, 0.f);
    (*triangle2)[2].position = sf::Vector2f(100.f, 100.f);
    (*triangle2)[0].color = sf::Color::Red;
    (*triangle2)[1].color = sf::Color::Green;
    (*triangle2)[2].color = sf::Color::Blue;

    auto g1 = std::make_shared<Player>(triangle, bricksTxt);
    g1->setOrigin({ 50.f, 50.f });
    g1->move({ 100, 100 });
    g1->rotate(sf::degrees(45));
    g1->scale({ 2.f, 2.f });
    engine.AddGameObject(g1);

    auto g2 = std::make_shared<shak::GameObject>(triangle2);
    g2->move({ 200, 200 });
    g1->AddChild(g2);

    engine.Start();
    return 0;
}