#pragma once

#include "EngineDefines.h"
#include "TextureAtlas.h"
#include "Sprite.h"

namespace shak
{
    class Animation : public shak::Sprite
    {
    public:
        Animation(std::shared_ptr<shak::TextureAtlas> atlas, float duration, const std::shared_ptr<sf::Shader>& shader = nullptr, const sf::Vector2f& size = { 400.f, 400.f }, const sf::Color tint = sf::Color::White);
        ~Animation() = default;

        void Play();

        void Update(float dt) override;

    private:
        std::shared_ptr<shak::TextureAtlas> m_atlas;
        float m_duration, m_time;
        bool m_playing;
        int m_frames, m_currentFrame;
    };
}