#include "ShakEngine.h"

#include "Player.h"
#include "ParticleSystem.h"
#include "Background.h"

#include "ShaderDevHelper.h"

int main()
{
    auto engine = std::make_shared<ShakEngine>();

    {
        auto& rm = engine->GetResourceManager();
        auto goliathPlus = rm.LoadTextureAtlas("assets/textures/New_Goliath_plus/ship106.atlas", "goliathPlus");

        auto camera1 = std::make_shared<shak::Camera>(sf::FloatRect({ 0, 0 }, { 1920, 1080 }));
        engine->AddCamera("camera1", camera1);


        // auto player = std::make_shared<Player>(engine, goliathPlus);
        // player->AddChild(camera1);
        // auto bg = std::make_shared<shak::Background>(rm.LoadTexture("assets/textures/bg1.jpg", "bg1", true), sf::Vector2f(1920.f, 1080.f));
        // camera1->SetBackground(bg);
        // auto bgsize = rm.GetTexture("bg1")->getSize();
        // camera1->SetBackgroundSize({ (float)(bgsize.x * 100), (float)(bgsize.y * 100) });

        // engine->AddGameObject(bg);

        // engine->AddGameObject(player);

        // camera1->move({ -1920 / 2, -1080 / 2 });

        // auto bricks = rm.LoadTexture("assets/textures/Bricks.jpg", "bricks");
        auto chicken = rm.LoadTexture("assets/textures/abstract1.png", "chicken", true, true);

        auto psShader = rm.LoadShader("assets/shaders/particle.vs", "assets/shaders/particle.fs", "particle");
        psShader->setUniform("u_texture", *chicken);
        psShader->setUniform("u_resolution", sf::Glsl::Vec2{ engine->GetWindowSize().x, engine->GetWindowSize().y });


        auto ps = std::make_shared<shak::ParticleSystem>(
            1, 60.f,  // count
            200.f, 200.f,   // lifetime
            0.f,       // start delay
            600.f, 600.f,  // size
            sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f),   // velocity 
            sf::Color::White, sf::Color::White, false,              // color
            shak::Particle::Type::Quad, chicken);           // type and texture
        ps->SetShader(psShader);
        ps->setPosition({ 0.f, 0.f });
        // engine->AddGameObject(ps);

        auto shaderHelper = std::make_shared<ShaderDevHelper>("particle", chicken);
        engine->AddGameObject(shaderHelper);
    }

    engine->Start();
    return 0;
}
