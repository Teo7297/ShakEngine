#pragma once

#include "ShakEngine.h"

class Player : public shak::GameObject
{
public:
    Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<sf::VertexArray> va, std::shared_ptr<shak::TextureAtlas> atlas)
        : GameObject(va, atlas->GetAtlasTexture()), m_engine(engine), m_atlas(atlas), m_atlasTexturesCount(atlas->GetCount())
    {
        m_bbSize = va->getBounds().size;
        this->setOrigin(va->getBounds().getCenter());
    }
    ~Player() = default;

    void HandleInput(const sf::Event& event) override
    {
        GameObject::HandleInput(event);
    }

    void Update(float dt) override
    {
        static int c = 0;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            auto mousePos = m_engine->GetMouseWorldPos();
            auto shipPos = getPosition();

            bool updateDestination = !this->IsPointInside(mousePos - shipPos + m_bbSize / 2.f);

            if (updateDestination)
                m_destination = mousePos;


        }

        if ((getPosition() - m_destination).lengthSquared() > 10.f)
        {
            // Move
            m_direction = m_destination - getPosition();
            m_direction = m_direction.normalized();
            this->move(m_direction * m_speed * dt);

            // Update texture
            auto coords = m_atlas->GetTextureCoords(GetTextureByDirection());
            (*m_vertices)[0].texCoords = coords.topLeft;
            (*m_vertices)[1].texCoords = coords.bottomLeft;
            (*m_vertices)[2].texCoords = coords.topRight;
            (*m_vertices)[3].texCoords = coords.bottomRight;

        }

        GameObject::Update(dt);
    }

private:
    int GetTextureByDirection() const
    {
        const auto directionAngle = m_direction.angleTo({ 1.f, 0.f }); // Distance from right dir (aka 0 degrees)
        float signedAngle = (directionAngle.asRadians() / (2.f * 3.14159265f));
        if (signedAngle < 0.f)
            signedAngle += 1.f;
        return static_cast<int>(signedAngle * m_atlasTexturesCount) % m_atlasTexturesCount;
    }

private:
    float m_speed = 1000.f;
    sf::Vector2f m_direction;
    sf::Vector2f m_destination;
    std::shared_ptr<ShakEngine> m_engine;

    sf::Vector2f m_bbSize;

    std::shared_ptr<shak::TextureAtlas> m_atlas;
    int m_atlasTexturesCount;
};
