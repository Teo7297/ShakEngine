#pragma once

#include "GameObject.h"

namespace shak
{
    class Circle : public GameObject
    {
    public:
        enum class Type
        {
            Filled,
            Outlined
        };

        Circle(const sf::Vector2f& center, float radius, const sf::Color& color = sf::Color::White, const Type& type = Type::Filled)
        {
            if (type == Type::Filled)
            {
                m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleFan, 257);
            }
            else
            {
                m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::LineStrip, 257);
            }

            (*m_vertices)[0].position = center + sf::Vector2f{ std::cos(0.f) * radius, std::sin(0.f) * radius };
            (*m_vertices)[0].color = color;
            for (int i = 1; i < 256; i++)
            {
                const float angle = 2.f * (float)M_PI * (float)i / 256.0f;
                (*m_vertices)[i].position = center + sf::Vector2f{ std::cos(angle) * radius, std::sin(angle) * radius };
                (*m_vertices)[i].color = color;
            }
            (*m_vertices)[256].position = center + sf::Vector2f{ std::cos(0.f) * radius, std::sin(0.f) * radius };
            (*m_vertices)[256].color = color;
        }
        ~Circle() override = default;


    private:
    };
}