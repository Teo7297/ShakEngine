#include "EngineDefines.h"

#include "Renderer.h"
#include "Scene.h"

int main()
{
    auto renderer = std::make_shared<shak::Renderer>();
    auto window = renderer->CreateSFWindow("GAME");
    renderer->Start();
    window->setFramerateLimit(170);

    shak::Scene scene(renderer);

    // Create a clock to measure deltaTime
    sf::Clock clock;

    sf::VertexArray triangle(sf::PrimitiveType::Triangles, 3);

    // define the position of the triangle's points
    triangle[0].position = sf::Vector2f(10.f, 10.f);
    triangle[1].position = sf::Vector2f(100.f, 10.f);
    triangle[2].position = sf::Vector2f(100.f, 100.f);

    // define the color of the triangle's points
    triangle[0].color = sf::Color::Red;
    triangle[1].color = sf::Color::Blue;
    triangle[2].color = sf::Color::Green;

    auto g1 = std::make_shared<shak::GameObject>(triangle);
    auto g2 = std::make_shared<shak::GameObject>(triangle);
    g1->setPosition({ 100.f, 100.f });
    g1->setScale({ 8.f, 8.f });
    // g1->setPosition({ 100.f, 100.f });
    g2->setScale({ 4.f, 4.f });

    std::filesystem::path vs = "shaders/test.vs";
    std::filesystem::path fs = "shaders/test.fs";
    auto shader = std::make_shared<sf::Shader>(vs, fs);
    g1->SetShader(shader);
    g2->SetShader(shader);
    scene.AddGameObject(g1);
    scene.AddGameObject(g2);

    auto mousePosPixel = sf::Vector2f{};

    // Movement
    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                renderer->Stop();
                exit(0);
            }
            else if (event->is<sf::Event::MouseMoved>())
            {
                mousePosPixel = { (float)sf::Mouse::getPosition(*window).x, DEFAULT_APP_HEIGHT - (float)sf::Mouse::getPosition(*window).y };
                // mousePosWorld = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
                shader->setUniform("mouse", mousePosPixel);
            }

            scene.HandleInput(event.value());
        }

        // Calculate delta time
        float dt = clock.restart().asSeconds();

        // Update the active scene
        scene.Update(dt);

        scene.Render();
    }

    return 0;
}