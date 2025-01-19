#include "Background.h"

namespace shak
{
    Background::Background(std::shared_ptr<sf::Texture> texture, sf::Vector2f screenSize)
        : m_screenSize(screenSize)
    {
        m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
        (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
        (*m_vertices)[1].position = sf::Vector2f(0.f, screenSize.y);
        (*m_vertices)[2].position = sf::Vector2f(screenSize.x, 0.f);
        (*m_vertices)[3].position = sf::Vector2f(screenSize.x, screenSize.y);
        (*m_vertices)[0].color = sf::Color::White;
        (*m_vertices)[1].color = sf::Color::White;
        (*m_vertices)[2].color = sf::Color::White;
        (*m_vertices)[3].color = sf::Color::White;

        m_texture = texture;
    }

    Background::~Background()
    {
    }

    void Background::Update(float dt)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        // Update texture coords with position
        // auto position = this->getPosition(); // I need the position of the camera

        // Convert camera global position to bg local position
        // sf::Vector2f local = -position;
        auto pos = m_screenCoords.position;
        auto size = m_screenCoords.size;


        // Tex coords are LOCAL
        (*m_vertices)[0].texCoords = sf::Vector2f(pos.x, pos.y);
        (*m_vertices)[1].texCoords = sf::Vector2f(pos.x, pos.y + m_screenSize.y);
        (*m_vertices)[2].texCoords = sf::Vector2f(pos.x + m_screenSize.x, pos.y);
        (*m_vertices)[3].texCoords = sf::Vector2f(pos.x + m_screenSize.x, pos.y + m_screenSize.y);
    }
}