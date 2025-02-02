#include "Background.h"

namespace shak
{
    Background::Background(const std::shared_ptr<sf::Texture> texture, const sf::Vector2f& screenSize)
    {
        m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
        
        this->SetScreenSize(screenSize);
        this->SetColor(sf::Color::White);
        m_texture = texture;
        this->m_zIndex = -100;
    }

    Background::~Background()
    {
    }

    void Background::SetScreenSize(const sf::Vector2f& size)
    {
        (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
        (*m_vertices)[1].position = sf::Vector2f(0.f, size.y);
        (*m_vertices)[2].position = sf::Vector2f(size.x, 0.f);
        (*m_vertices)[3].position = sf::Vector2f(size.x, size.y);
        m_screenSize = size;
    }

    void Background::SetColor(const sf::Color& color)
    {
        (*m_vertices)[0].color = sf::Color::White;
        (*m_vertices)[1].color = sf::Color::White;
        (*m_vertices)[2].color = sf::Color::White;
        (*m_vertices)[3].color = sf::Color::White;
    }

    void Background::Update(float dt)
    {
        auto pos = m_screenCoords.position;
        auto size = m_screenCoords.size;

        // Tex coords are LOCAL
        (*m_vertices)[0].texCoords = sf::Vector2f(pos.x, pos.y);
        (*m_vertices)[1].texCoords = sf::Vector2f(pos.x, pos.y + m_screenSize.y);
        (*m_vertices)[2].texCoords = sf::Vector2f(pos.x + m_screenSize.x, pos.y);
        (*m_vertices)[3].texCoords = sf::Vector2f(pos.x + m_screenSize.x, pos.y + m_screenSize.y);
    }
}