// #include "EngineDefines.h"

// #include "Renderer.h"


// int main_example()
// {
//     auto renderer = std::make_shared<shak::Renderer>();
//     auto window = renderer->CreateSFWindow("Shak Engine Demo");
//     renderer->Start();
//     window->setFramerateLimit(170);

//     auto texture = std::make_shared<sf::Texture>("textures/bricks.jpg");
//     auto sprite = std::make_shared<sf::Sprite>(*texture);
//     sprite->setScale({0.2f, 0.2f});

//     sf::Shader triangleShader;
//     bool loaded = triangleShader.loadFromFile("shaders/test.vs", "shaders/test.fs");
    
//     sf::View camera(sf::FloatRect({200.f, 200.f}, {300.f, 200.f}));
//     // camera.setViewport(sf::FloatRect({0.25f, 0.25}, {0.5f, 0.5f}));

//     // Music test
//     sf::Music music("audio/AgainstAllOdds.wav");
//     music.setVolume(30.f);
//     // music.play();

//     // Pixel mouse position, useful for shaders
//     auto mousePosPixel = sf::Vector2f{};
//     // World mouse position, useful for CPU calculations
//     auto mousePosWorld = sf::Vector2f{};

//     while (window->isOpen())
//     {
//         while (const std::optional event = window->pollEvent())
//         {
//             if (event->is<sf::Event::Closed>())
//             {
//                 renderer->Stop();
//                 exit(0);
//             }

//             else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
//             {
//                 switch (keyPressed->scancode)
//                 {
//                     case sf::Keyboard::Scancode::Escape:
//                         renderer->Stop();
//                         exit(0);
//                         break;
//                     case sf::Keyboard::Scancode::Up:
//                         sprite->setScale({sprite->getScale().x * 2, sprite->getScale().y * 2});
//                         break;
//                     case sf::Keyboard::Scancode::Down:
//                         sprite->setScale({sprite->getScale().x / 2, sprite->getScale().y / 2});
//                         break;
//                     default:
//                         break;
//                 }
//             }
//             else if(const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>())
//             {
//                 if(wheel->delta > 0)
//                 {
//                     camera.zoom(0.9f);
//                 }
//                 else
//                 {
//                     camera.zoom(1.1f);
//                 }
//             }
//             else if (event->is<sf::Event::MouseMoved>())
//             {
//                 mousePosPixel = {(float)sf::Mouse::getPosition(*window).x, DEFAULT_APP_HEIGHT - (float)sf::Mouse::getPosition(*window).y};
//                 mousePosWorld = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
//             }
//         }
        
//         // Camera Movement
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
//             camera.setCenter({camera.getCenter().x, camera.getCenter().y - 20});
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
//             camera.setCenter({camera.getCenter().x, camera.getCenter().y + 20});
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
//             camera.setCenter({camera.getCenter().x - 20, camera.getCenter().y});
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
//             camera.setCenter({camera.getCenter().x + 20, camera.getCenter().y});

//         std::vector<shak::Drawable> drawables;
//         auto spriteStates = std::make_shared<sf::RenderStates>();;
//         drawables.emplace_back(sprite, spriteStates);

//         // create an array of 3 vertices that define a triangle primitive
//         auto triangle = std::make_shared<sf::VertexArray>(sf::PrimitiveType::Triangles, 3);
//         (*triangle)[0].position = sf::Vector2f(150.f, 150.f);
//         (*triangle)[1].position = sf::Vector2f(1050.f, 150.f);
//         (*triangle)[2].position = sf::Vector2f(1050.f, 1050.f);
//         (*triangle)[0].color = sf::Color::Red;
//         (*triangle)[1].color = sf::Color::Green;
//         (*triangle)[2].color = sf::Color::Blue;
//         (*triangle)[0].texCoords = sf::Vector2f(0,0);
//         (*triangle)[1].texCoords = sf::Vector2f(texture->getSize().x, 0);
//         (*triangle)[2].texCoords = sf::Vector2f(texture->getSize().x, texture->getSize().y);

//         auto triangleStates = std::make_shared<sf::RenderStates>();
//         triangleStates->texture = &*texture;

//         triangleShader.setUniform("texture", sf::Shader::CurrentTexture);
//         triangleShader.setUniform("mouse", sf::Glsl::Vec2{ mousePosPixel });
//         triangleStates->shader = &triangleShader;
//         spriteStates->shader = &triangleShader;
//         drawables.emplace_back(triangle, triangleStates);

//         sprite->setPosition(mousePosWorld);

//         renderer->PushToRenderQueue(drawables);


//         // Last, update camera view
//         window->setView(camera);

//     }

//     return 0;
// }
