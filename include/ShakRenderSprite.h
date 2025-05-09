#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    class RenderSprite : public GameObject
    {
    public:
        RenderSprite(const std::shared_ptr<sf::RenderTexture>& renderTexture, const std::shared_ptr<sf::Shader>& shader = nullptr, const sf::Vector2f& size = { 100.f, 100.f }, const sf::Color tint = sf::Color::White)
            : GameObject()
        {
            m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
            (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
            (*m_vertices)[1].position = sf::Vector2f(0.f, size.y);
            (*m_vertices)[2].position = sf::Vector2f(size.x, 0.f);
            (*m_vertices)[3].position = sf::Vector2f(size.x, size.y);
            this->SetColor(tint);

            if(renderTexture)
            {
                auto txtSize = renderTexture->getSize();
                (*m_vertices)[0].texCoords = { 0.f, 0.f };
                (*m_vertices)[1].texCoords = { 0.f, (float)txtSize.y };
                (*m_vertices)[2].texCoords = { (float)txtSize.x, 0.f };
                (*m_vertices)[3].texCoords = { (float)txtSize.x, (float)txtSize.y };
            }
            else
            {
                std::cerr << "[RenderSprite] RenderTexture is null" << std::endl;
            }

            m_renderTexture = renderTexture;
            m_shader = shader;

            this->setOrigin({ size.x / 2.f, size.y / 2.f });

            this->SetRenderTarget(true);
        }

        virtual ~RenderSprite() = default;

    protected:
    };
}