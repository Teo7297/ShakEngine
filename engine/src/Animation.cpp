#include "Animation.h"


namespace shak
{
    Animation::Animation(std::shared_ptr<shak::TextureAtlas> atlas, float duration, const std::shared_ptr<sf::Shader>& shader, const sf::Vector2f& size, const sf::Color tint)
        : shak::Sprite(atlas->GetAtlasTexture(), shader, size, { 0,0,0,0 }), m_atlas(atlas), m_duration(duration), m_time(0.f), m_playing(false), m_frames(atlas->GetCount()), m_currentFrame(0)
    {
    }

    void Animation::Play()
    {
        m_playing = true;
        (*m_vertices)[0].color = sf::Color::White;
        (*m_vertices)[1].color = sf::Color::White;
        (*m_vertices)[2].color = sf::Color::White;
        (*m_vertices)[3].color = sf::Color::White;
    }

    void Animation::Update(float dt)
    {
        if (!m_playing) return;

        m_time += dt;
        if (m_time >= m_duration)
        {
            m_time = 0.f;
            (*m_vertices)[0].color = {0,0,0,0};
            (*m_vertices)[1].color = {0,0,0,0};
            (*m_vertices)[2].color = {0,0,0,0};
            (*m_vertices)[3].color = {0,0,0,0};
            m_playing = false;
        }

        auto currentFrame = std::floor(m_time / (m_duration / m_frames));
        if (m_currentFrame != currentFrame)
        {
            m_currentFrame = currentFrame;
            auto frameCoords = m_atlas->GetTextureCoords(m_currentFrame);
            (*m_vertices)[0].texCoords = frameCoords.topLeft;
            (*m_vertices)[1].texCoords = frameCoords.bottomLeft;
            (*m_vertices)[2].texCoords = frameCoords.topRight;
            (*m_vertices)[3].texCoords = frameCoords.bottomRight;
        }

    }



}