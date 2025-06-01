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

        Square(const sf::Vector2f& size, const sf::Color& color = sf::Color::White, const Type& type = Type::Filled, bool centerOrigin = false)
        {
            if(type == Type::Filled)
            {
                m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleFan, 4);
                (*m_vertices)[0].position = { 0.f, 0.f };
                (*m_vertices)[1].position = { size.x, 0.f };
                (*m_vertices)[2].position = { size.x, size.y };
                (*m_vertices)[3].position = { 0.f, size.y };
                for(int i = 0; i < 4; i++)
                {
                    (*m_vertices)[i].color = color;
                }
            }
            else
            {
                m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::LineStrip, 5);
                (*m_vertices)[0].position = { 0.f, 0.f };
                (*m_vertices)[1].position = { size.x, 0.f };
                (*m_vertices)[2].position = { size.x, size.y };
                (*m_vertices)[3].position = { 0.f, size.y };
                (*m_vertices)[4].position = { 0.f, 0.f };
                for(int i = 0; i < 5; i++)
                {
                    (*m_vertices)[i].color = color;
                }
            }

            if(centerOrigin)
                this->setOrigin(size / 2.f);
        }
        ~Square() override = default;


    private:
    };
}