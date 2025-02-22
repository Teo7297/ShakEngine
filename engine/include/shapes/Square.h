#pragma once

#include "GameObject.h"

namespace shak
{
    class Square : public GameObject
    {
    public:
        enum class Type
        {
            Filled,
            Outlined
        };

        Square(const sf::FloatRect& square, const sf::Color& color = sf::Color::White, const Type& type = Type::Filled)
        {
            if (type == Type::Filled)
            {
                m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 6);
                (*m_vertices)[0].position = { square.position.x, square.position.y };
                (*m_vertices)[1].position = { square.position.x + square.size.x, square.position.y };
                (*m_vertices)[2].position = { square.position.x, square.position.y + square.size.y };
                (*m_vertices)[3].position = { square.position.x + square.size.x, square.position.y };
                (*m_vertices)[4].position = { square.position.x + square.size.x, square.position.y + square.size.y };
                (*m_vertices)[5].position = { square.position.x, square.position.y + square.size.y };
                for (int i = 0; i < 6; i++)
                {
                    (*m_vertices)[i].color = color;
                }
            }
            else
            {
                m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::LineStrip, 5);
                (*m_vertices)[0].position = { square.position.x, square.position.y };
                (*m_vertices)[1].position = { square.position.x + square.size.x, square.position.y };
                (*m_vertices)[2].position = { square.position.x + square.size.x, square.position.y + square.size.y };
                (*m_vertices)[3].position = { square.position.x, square.position.y + square.size.y };
                (*m_vertices)[4].position = { square.position.x, square.position.y };
                for (int i = 0; i < 5; i++)
                {
                    (*m_vertices)[i].color = color;
                }
            }
        }
        ~Square() override = default;


    private:
    };
}