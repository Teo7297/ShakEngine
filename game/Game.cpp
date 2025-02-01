#include "ShakEngine.h"

#include "Player.h"
#include "ParticleSystem.h"
#include "Background.h"

#include "Alien.h"
#include "Animation.h"

#include "ShaderDevHelper.h"

int main()
{
    auto engine = std::make_shared<ShakEngine>();

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

        auto player = std::make_shared<Player>(engine, goliathPlus, laserTxt, laserSh, explosionAtlas);
        player->AddChild(camera1);
        auto bg = std::make_shared<shak::Background>(rm.LoadTexture("assets/textures/bg1.jpg", "bg1", true), sf::Vector2f(1920.f, 1080.f));
        camera1->SetBackground(bg);
        auto bgsize = rm.GetTexture("bg1")->getSize();
        camera1->SetBackgroundSize({ (float)(bgsize.x * 100), (float)(bgsize.y * 100) });

        engine->AddGameObject(bg);

        engine->AddGameObject(player);

        camera1->move(player->GetVertexArray()->getBounds().size / 2.f);
        // auto chicken = rm.LoadTexture("assets/textures/abstract1.png", "chicken", true, true);
        // auto psShader = rm.LoadShader("", "assets/shaders/laserShot.fs", "particle");
        // psShader->setUniform("u_texture", *chicken);
        // psShader->setUniform("u_resolution", sf::Glsl::Vec2{ engine->GetWindowSize().x, engine->GetWindowSize().y });
        // auto ps = std::make_shared<shak::ParticleSystem>(
        //     2000, 600.f,  // count
        //     .5f, 2.f,   // lifetime
        //     0.f,       // start delay
        //     600.f, 600.f,  // size
        //     sf::Vector2f(-200.f, -200.f), sf::Vector2f(200.f, 200.f),   // velocity 
        //     sf::Color::Green, sf::Color::Red, false,              // color
        //     shak::Particle::Type::Point, nullptr);           // type and texture
        // ps->SetShader(psShader);
        // ps->setPosition(player->getPosition());
        // player->AddChild(ps);
        // engine->AddGameObject(ps);

        // for(int i = 0; i < 100; i++)
        // {
        auto alien = std::make_shared<Alien>(goliathPlus, player, explosionAtlas);
        engine->AddGameObject(alien);
        alien->setPosition({ 500.f, 500.f });
        alien->Name = "Alien";
        // }        

        // auto shaderHelper = std::make_shared<ShaderDevHelper>("particle", chicken);
        // engine->AddGameObject(shaderHelper);

    }

    engine->Start();
    return 0;
}
