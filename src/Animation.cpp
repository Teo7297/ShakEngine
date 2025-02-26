#include "Animation.h"


namespace shak
{
    Animation::Animation(std::shared_ptr<shak::TextureAtlas> atlas, float duration, const std::shared_ptr<sf::Shader>& shader, const sf::Vector2f& size, const sf::Color tint)
        : shak::Sprite(atlas->GetAtlasTexture(), shader, size, tint)
        , m_atlas(atlas)
        , m_duration(duration)
        , m_time(0.f)
        , m_playing(false)
        , m_frames(atlas->GetCount())
        , m_currentFrame(-1)
        , m_tint(tint)
    {
        this->SetTransparency(0);
    }

    void Animation::Play()
    {
        m_time = 0.f;
        m_playing = true;
    }

    void Animation::Stop()
    {
        m_time = m_duration + 1.f;
    }

    void Animation::Pause()
    {
        m_playing = false;
    }

    void Animation::Resume()
    {
        m_playing = true;
    }

    void Animation::Update(float dt)
    {
        if (!m_playing) return;

        m_time += dt;
        if (m_time >= m_duration)
        {
            m_time = 0.f;
            this->SetTransparency(0);
            m_playing = false;
            OnAnimationEnd();
            return;
        }

        int currentFrame = static_cast<int>(std::floor(m_time * (static_cast<float>(m_frames) / m_duration)));
        if (m_currentFrame != currentFrame)
        {
            this->SetTransparency(255);
            m_currentFrame = currentFrame;
            const auto frameCoords = m_atlas->GetTextureCoords(m_currentFrame);
            this->SetTextureCoordinates(frameCoords);
        }

    }

    void Animation::SetTextureCoordinates(const shak::TextureCoordinates& coords) const
    {
        (*m_vertices)[0].texCoords = coords.topLeft;
        (*m_vertices)[1].texCoords = coords.bottomLeft;
        (*m_vertices)[2].texCoords = coords.topRight;
        (*m_vertices)[3].texCoords = coords.bottomRight;
    }
}