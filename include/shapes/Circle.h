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
            : m_radius{ radius }
        {
            if(type == Type::Filled)
            {
                m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleFan, m_verticesCount);
            }
            else
            {
                m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::LineStrip, m_verticesCount);
            }

            (*m_vertices)[0].position = center + sf::Vector2f{ std::cos(0.f) * radius, std::sin(0.f) * radius };
            (*m_vertices)[0].color = color;
            for(int i = 1; i < m_verticesCount - 1; i++)
            {
                const float angle = 2.f * (float)M_PI * (float)i / (float)(m_verticesCount - 1);
                (*m_vertices)[i].position = center + sf::Vector2f{ std::cos(angle) * radius, std::sin(angle) * radius };
                (*m_vertices)[i].color = color;
            }
            (*m_vertices)[m_verticesCount - 1].position = center + sf::Vector2f{ std::cos(0.f) * radius, std::sin(0.f) * radius };
            (*m_vertices)[m_verticesCount - 1].color = color;
        }
        ~Circle() override = default;


    protected:
        const int m_verticesCount = 257;
        float m_radius;
    };
}