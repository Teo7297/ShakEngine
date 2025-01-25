#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    class Sprite : public GameObject
    {
    public:
        Sprite(const std::shared_ptr<sf::Texture>& texture, const std::shared_ptr<sf::Shader>& shader = nullptr, const sf::Vector2f& size = { 100.f, 100.f }, const sf::Color tint = sf::Color::White)
            : GameObject()
        {
            auto txtSize = texture->getSize();
            m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
            (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
            (*m_vertices)[1].position = sf::Vector2f(0.f, size.y);
            (*m_vertices)[2].position = sf::Vector2f(size.x, 0.f);
            (*m_vertices)[3].position = sf::Vector2f(size.x, size.y);
            (*m_vertices)[0].color = tint;
            (*m_vertices)[1].color = tint;
            (*m_vertices)[2].color = tint;
            (*m_vertices)[3].color = tint;
            (*m_vertices)[0].texCoords = { 0.f, 0.f };
            (*m_vertices)[1].texCoords = { 0.f, (float)txtSize.y };
            (*m_vertices)[2].texCoords = { (float)txtSize.x, 0.f };
            (*m_vertices)[3].texCoords = { (float)txtSize.x, (float)txtSize.y };

            m_texture = texture;
            m_shader = shader;

            this->setOrigin(m_vertices->getBounds().getCenter());
        }

        virtual ~Sprite() = default;

    protected:
    };
}