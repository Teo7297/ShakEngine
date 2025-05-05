#pragma once

#include "Scene.h"
#include "Camera.h"
#include "shapes/Square.h"

#pragma once

#include "GameObject.h"


#pragma once

#include "GameObject.h"

class EngineLogoText : public shak::GameObject
{
public:
    EngineLogoText()
        : GameObject()
    {
    }

    ~EngineLogoText() override
    {
    }

    void Awake() override
    {
        m_zIndex = 1000;
        m_font = m_engine->GetResourceManager()->LoadFont("assets/fonts/Sigmar-Regular.ttf", "sigmar");
        m_text = std::make_shared<sf::Text>(*m_font);
        m_text->setString("SHAKENGINE");
        m_text->setCharacterSize(100);
        m_text->setOrigin({ m_text->getLocalBounds().size.x / 2.f, m_text->getLocalBounds().size.y / 2.f });
        m_text->setFillColor(sf::Color::White);
        m_text->setOutlineThickness(1.f);
        m_text->setOutlineColor(sf::Color::Black);
    }

    void Update(float dt) override
    {
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= this->getTransform();
        target.draw(*m_text, states);
    }

    std::shared_ptr<sf::Text> GetText() const { return m_text; }

private:
    std::shared_ptr<sf::Text> m_text;
    std::shared_ptr<sf::Font> m_font;
};

class LogoRoot : public shak::GameObject
{
public:
    LogoRoot()
        : GameObject()
    {
    }

    void Awake() override
    {
        m_sound = m_engine->GetResourceManager()->LoadSound("assets/sounds/shakengine_startup.wav", "logo_sound");
        m_sound->setVolume(75.f);
        m_sound->play();
    }

    void Update(float dt) override
    {

        if (m_sound->getStatus() == sf::Sound::Status::Stopped)
        {
            m_fading = true;
        }

        if (m_fading)
        {
            m_alpha -= (m_fadeSpeed * dt);
            if (m_alpha <= 0.f)
            {
                LoadMainScene();
            }
            else
            {
                auto logo = std::dynamic_pointer_cast<shak::Square>(FindChildRecursive("logo"));
                logo->SetTransparency(m_alpha);

                auto text = std::dynamic_pointer_cast<EngineLogoText>(FindChildRecursive("text"));
                auto textFillColor = text->GetText()->getFillColor();
                textFillColor.a = m_alpha;
                text->GetText()->setFillColor(textFillColor);
                auto textOutlineColor = text->GetText()->getOutlineColor();
                textOutlineColor.a = m_alpha;
                text->GetText()->setOutlineColor(textOutlineColor);
            }
        }
    }

    void HandleInput(const sf::Event& event) override
    {
        if (event.is<sf::Event::KeyPressed>())
        {
            LoadMainScene();
        }
    }

private:
    void LoadMainScene()
    {
        m_engine->GetSceneManager()->QueueNextScene();
    }

    std::shared_ptr<sf::Sound> m_sound;
    bool m_fading = false;
    uint8_t m_alpha = 255;
    float m_fadeSpeed = 2.f;
};


class EngineLogoSplash : public shak::Scene
{
public:
    EngineLogoSplash(const std::string& name) : Scene(name)
    {
    }
    ~EngineLogoSplash() override
    {
    }

    void Init() override
    {
        auto camera1 = m_renderer->AddCamera("camera1", sf::FloatRect({ 0, 0 }, { 1920, 1080 }), shak::CameraResizeBehavior::Letterbox);

        auto root = m_engine->AddGameObject<LogoRoot>();
        root->Name = "logoroot";

        auto quad = std::make_shared<shak::Square>(sf::FloatRect({ 0, 0 }, { 1920, 1080 }), sf::Color::White, shak::Square::Type::Filled);
        quad->Name = "logo";
        quad->move(sf::Vector2f{ 1920 / 2, 1080 / 2 });
        quad->SetShader(m_engine->GetResourceManager()->LoadShader("", "shaders/logo.fs", "logo"));
        root->AddChild(quad);

        auto text = std::make_shared<EngineLogoText>();
        text->Name = "text";
        text->setPosition(sf::Vector2f{ 1920 / 2, 1080 / 2 });
        root->AddChild(text);
    }

    void Clear() override
    {
    }
};