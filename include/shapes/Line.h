#pragma once

#include "GameObject.h"

namespace shak
{
    class Line : public GameObject
    {
    public:
        Line(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color = sf::Color::White)
        {
            m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::Lines, 2);
            (*m_vertices)[0].position = start;
            (*m_vertices)[1].position = end;
            (*m_vertices)[0].color = color;
            (*m_vertices)[1].color = color;
        }
        ~Line() override = default;


    private:
    };
}