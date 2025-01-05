// SFML modules
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

// SFML wrappers/utilities 
#include <SFML/OpenGL.hpp>

// STL
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>

#define DEFAULT_APP_WIDTH 1920U
#define DEFAULT_APP_HEIGHT 1080U

namespace shak
{
    struct Drawable
    {
        Drawable(std::shared_ptr<sf::Drawable> d, std::shared_ptr<sf::RenderStates> s) : drawable{d}, renderStates{s}{}
        ~Drawable(){}
        std::shared_ptr<sf::Drawable> drawable;
        std::shared_ptr<sf::RenderStates> renderStates;
    };
}

std::queue<shak::Drawable> renderQueue;
std::mutex renderQueueMutex;
std::condition_variable condition;
bool isRunning = true;

void pushToRenderQueue(const std::vector<shak::Drawable>& drawables) 
{
    std::lock_guard<std::mutex> lock(renderQueueMutex);
    for(const auto& drawable : drawables)
        renderQueue.push(drawable);
    condition.notify_one(); // Notify the rendering thread
}

void clearRenderQueue() 
{
    std::lock_guard<std::mutex> lock(renderQueueMutex);
    while(!renderQueue.empty())
        renderQueue.pop();
    condition.notify_one(); // Notify the rendering thread
}

void renderingThread(sf::RenderWindow* window)
{
    // activate the window's context
    if(!window->setActive(true))
        std::cerr <<"[Render thread] OpenGL context could not be activated" << std::endl;


    // the rendering loop
    while (isRunning)
    {
        std::unique_lock<std::mutex> lock(renderQueueMutex);

        // Wait for items to draw or for the application to close
        condition.wait(lock, [=] { return !renderQueue.empty() || !isRunning; });

        window->clear({2, 38, 46});
        while (!renderQueue.empty()) 
        {
            const auto [drawable, renderStates] = renderQueue.front();
            renderQueue.pop();
            window->draw(*drawable, *renderStates);
        }

        window->display();
    }

    // The render thread has the authority over the window, we could also deactivate it here and close it from the main thread I guess...
    window->close();
}

void closeApp(sf::RenderWindow& window, std::thread& renderThread)
{
    clearRenderQueue();
    isRunning = false;
    condition.notify_all();
    renderThread.join();
    exit(0);
}

int main()
{
    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    auto window = sf::RenderWindow(sf::VideoMode({DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT}), "ShakEngine Demo");
    window.setFramerateLimit(170);

    // deactivate its OpenGL context, we are going to enable it in the render-thread
    if(!window.setActive(false))
        std::cerr << "[Main thread] OpenGL context could not be deactivated" << std::endl;

    // launch the rendering thread
    std::thread renderThread(&renderingThread, &window);

    auto texture = std::make_shared<sf::Texture>("textures/bricks.jpg");
    auto sprite = std::make_shared<sf::Sprite>(*texture);
    sprite->setScale({0.2f, 0.2f});

    sf::Shader triangleShader;
    bool loaded = triangleShader.loadFromFile("shaders/test.vs", "shaders/test.fs");
    
    sf::View camera(sf::FloatRect({200.f, 200.f}, {300.f, 200.f}));
    // camera.setViewport(sf::FloatRect({0.25f, 0.25}, {0.5f, 0.5f}));

    // Music test
    sf::Music music("audio/AgainstAllOdds.wav");
    music.setVolume(30.f);
    // music.play();

    // Pixel mouse position, useful for shaders
    auto mousePosPixel = sf::Vector2f{};
    // World mouse position, useful for CPU calculations
    auto mousePosWorld = sf::Vector2f{};

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                closeApp(window, renderThread);
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                switch (keyPressed->scancode)
                {
                    case sf::Keyboard::Scancode::Escape:
                        closeApp(window, renderThread);
                        break;
                    case sf::Keyboard::Scancode::Up:
                        sprite->setScale({sprite->getScale().x * 2, sprite->getScale().y * 2});
                        break;
                    case sf::Keyboard::Scancode::Down:
                        sprite->setScale({sprite->getScale().x / 2, sprite->getScale().y / 2});
                        break;
                    default:
                        break;
                }
            }
            else if(const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if(wheel->delta > 0)
                {
                    camera.zoom(0.9f);
                }
                else
                {
                    camera.zoom(1.1f);
                }
            }
            else if (event->is<sf::Event::MouseMoved>())
            {
                mousePosPixel = {(float)sf::Mouse::getPosition(window).x, DEFAULT_APP_HEIGHT - (float)sf::Mouse::getPosition(window).y};
                mousePosWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            }
        }
        
        // Camera Movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            camera.setCenter({camera.getCenter().x, camera.getCenter().y - 20});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            camera.setCenter({camera.getCenter().x, camera.getCenter().y + 20});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            camera.setCenter({camera.getCenter().x - 20, camera.getCenter().y});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            camera.setCenter({camera.getCenter().x + 20, camera.getCenter().y});

        std::vector<shak::Drawable> drawables;
        auto spriteStates = std::make_shared<sf::RenderStates>();;
        drawables.emplace_back(sprite, spriteStates);

        // create an array of 3 vertices that define a triangle primitive
        auto triangle = std::make_shared<sf::VertexArray>(sf::PrimitiveType::Triangles, 3);
        (*triangle)[0].position = sf::Vector2f(150.f, 150.f);
        (*triangle)[1].position = sf::Vector2f(1050.f, 150.f);
        (*triangle)[2].position = sf::Vector2f(1050.f, 1050.f);
        (*triangle)[0].color = sf::Color::Red;
        (*triangle)[1].color = sf::Color::Green;
        (*triangle)[2].color = sf::Color::Blue;
        (*triangle)[0].texCoords = sf::Vector2f(0,0);
        (*triangle)[1].texCoords = sf::Vector2f(texture->getSize().x, 0);
        (*triangle)[2].texCoords = sf::Vector2f(texture->getSize().x, texture->getSize().y);

        auto triangleStates = std::make_shared<sf::RenderStates>();
        triangleStates->texture = &*texture;

        triangleShader.setUniform("texture", sf::Shader::CurrentTexture);
        triangleShader.setUniform("mouse", sf::Glsl::Vec2{ mousePosPixel });
        triangleStates->shader = &triangleShader;
        spriteStates->shader = &triangleShader;
        drawables.emplace_back(triangle, triangleStates);

        sprite->setPosition(mousePosWorld);

        pushToRenderQueue(drawables);


        // Last, update camera view
        window.setView(camera);

    }

    return 0;
}
