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
        auto bgsize = rm.GetTexture("bg1")->getSize();
        camera1->SetBackgroundSize({ (float)(bgsize.x * 100), (float)(bgsize.y * 100) });

        engine->AddGameObject(bg);

        engine->AddGameObject(player);

        camera1->move({ -1920 / 2, -1080 / 2 });

        // auto bricks = rm.LoadTexture("assets/textures/Bricks.jpg", "bricks");
        auto chicken = rm.LoadTexture("assets/textures/Portrait.png", "chicken");
        auto ps = std::make_shared<shak::ParticleSystem>(600, 0.1f, .5f, 3.f, 2.f, 20.f, sf::Vector2(-200.f, -200.f), sf::Vector2f(200.f, 200.f), sf::Color::Blue, sf::Color::Green, chicken);

        ps->setPosition(player->getPosition());

        player->AddChild(ps);
    }

    engine->Start();
    return 0;
}

// sf::Texture texture;

// class ParticleSystem : public sf::Drawable, public sf::Transformable
// {
// public:
//     ParticleSystem(unsigned int count) : m_particles(count), m_vertices(sf::PrimitiveType::Triangles, count * 3)
//     {
//     }

//     void setEmitter(sf::Vector2f position)
//     {
//         m_emitter = position;
//     }

//     void update(sf::Time elapsed)
//     {
//         for (std::size_t i = 0; i < m_particles.size(); i += 6)
//         {
//             // update the particle lifetime
//             Particle& p = m_particles[i];
//             p.lifetime -= elapsed;

//             // if the particle is dead, respawn it
//             if (p.lifetime <= sf::Time::Zero)
//             {
//                 resetParticle(i);
//                 resetParticle(i + 1);
//                 resetParticle(i + 2);
//                 resetParticle(i + 3);
//                 resetParticle(i + 4);
//                 resetParticle(i + 5);
//             }

//             // update the position of the corresponding vertex
//             auto psize = 50.f;
//             m_vertices[i + 0].position += (p.velocity + sf::Vector2f{ 0.f, 0.f }) * elapsed.asSeconds();
//             m_vertices[i + 1].position += (p.velocity + sf::Vector2f{ 0.f, psize }) * elapsed.asSeconds();
//             m_vertices[i + 2].position += (p.velocity + sf::Vector2f{ psize, 0.f }) * elapsed.asSeconds();
//             m_vertices[i + 3].position += (p.velocity + sf::Vector2f{ psize, 0.f }) * elapsed.asSeconds();
//             m_vertices[i + 4].position += (p.velocity + sf::Vector2f{ 0.f, psize }) * elapsed.asSeconds();
//             m_vertices[i + 5].position += (p.velocity + sf::Vector2f{ psize, psize }) * elapsed.asSeconds();

//             // update the alpha (transparency) of the particle according to its lifetime
//             float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
//             m_vertices[i].color.a = static_cast<std::uint8_t>(ratio * 255);
//             m_vertices[i + 1].color.a = static_cast<std::uint8_t>(ratio * 255);
//             m_vertices[i + 2].color.a = static_cast<std::uint8_t>(ratio * 255);
//             m_vertices[i + 3].color.a = static_cast<std::uint8_t>(ratio * 255);
//             m_vertices[i + 4].color.a = static_cast<std::uint8_t>(ratio * 255);
//             m_vertices[i + 5].color.a = static_cast<std::uint8_t>(ratio * 255);

//             // texture coords
//             float w = texture.getSize().x;
//             float h = texture.getSize().y;
//             m_vertices[i].texCoords = { 0.f, 0.f };
//             m_vertices[i + 1].texCoords = { 0.f, h };
//             m_vertices[i + 2].texCoords = { w, 0.f };
//             m_vertices[i + 3].texCoords = { w, 0.f };
//             m_vertices[i + 4].texCoords = { 0.f, h };
//             m_vertices[i + 5].texCoords = { w, h };
//         }
//     }

// private:
//     void draw(sf::RenderTarget& target, sf::RenderStates states) const override
//     {
//         // apply the transform
//         states.transform *= getTransform();

//         // our particles don't use a texture
//         states.texture = &texture;

//         // draw the vertex array
//         target.draw(m_vertices, states);
//     }

//     struct Particle
//     {
//         sf::Vector2f velocity;
//         sf::Time     lifetime;
//     };

//     void resetParticle(std::size_t index)
//     {
//         // create random number generator
//         static std::random_device rd;
//         static std::mt19937       rng(rd());

//         // give a random velocity and lifetime to the particle
//         const sf::Angle angle = sf::degrees(std::uniform_real_distribution(0.f, 360.f)(rng));
//         const float     speed = std::uniform_real_distribution(50.f, 100.f)(rng);
//         m_particles[index].velocity = sf::Vector2f(speed, angle);
//         m_particles[index].lifetime = sf::milliseconds(std::uniform_int_distribution(1000, 3000)(rng));

//         // reset the position of the corresponding vertex
//         m_vertices[index].position = m_emitter;
//     }

//     std::vector<Particle> m_particles;
//     sf::VertexArray       m_vertices;
//     sf::Time              m_lifetime{ sf::seconds(3) };
//     sf::Vector2f          m_emitter;
// };

// int main()
// {
//     // create the window
//     sf::RenderWindow window(sf::VideoMode({ 512, 256 }), "Particles");

//     texture.loadFromFile("assets/textures/Portrait.png");

//     // create the particle system
//     ParticleSystem particles(120000);

//     // create a clock to track the elapsed time
//     sf::Clock clock;

//     // run the main loop
//     while (window.isOpen())
//     {
//         // handle events
//         while (const std::optional event = window.pollEvent())
//         {
//             if (event->is<sf::Event::Closed>())
//                 window.close();
//         }

//         // make the particle system emitter follow the mouse
//         sf::Vector2i mouse = sf::Mouse::getPosition(window);
//         particles.setEmitter(window.mapPixelToCoords(mouse));

//         // update it
//         sf::Time elapsed = clock.restart();
//         particles.update(elapsed);

//         // draw it
//         window.clear();
//         window.draw(particles);
//         window.display();
//     }
// }