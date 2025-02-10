#pragma once

#include "EngineDefines.h"

namespace shak
{
    struct Particle
    {
        enum class Type
        {
            Quad,
            Point
        };

        Particle(Particle::Type type)
            : vertices()
            , velocity({ 0.f, 0.f })
            , lifeTime(0.f)
            , maxLifeTime(0.f)
            , startColor(sf::Color::White)
            , endColor(sf::Color::White)
            , active(false)
            , id(0)
            , size(0.f)
        {
            if (type == Particle::Type::Quad)
                vertices.resize(6);
            else
                vertices.resize(1);
        }

        void Move(sf::Vector2f offset)
        {
            for (auto& v : vertices)
                v->position += offset;
        }

        void SetColor(sf::Color color)
        {
            for (auto& v : vertices)
                v->color = color;
        }

        void SetAlpha(uint8_t alpha)
        {
            for (auto& v : vertices)
                v->color.a = alpha;
        }

        void SetTextureCoords(float x, float y)
        {
            if (vertices.size() < 6)
                return;

            vertices[0]->texCoords = { 0.f, 0.f };
            vertices[1]->texCoords = { 0.f, y };
            vertices[2]->texCoords = { x, 0.f };
            vertices[3]->texCoords = { x, 0.f };
            vertices[4]->texCoords = { 0.f, y };
            vertices[5]->texCoords = { x, y };
        }

        void SetVertices(const std::shared_ptr<sf::VertexArray>& va, int index)
        {
            if (vertices.size() < 6)
            {
                vertices[0] = &(*va)[index];
                return;
            }

            int j = index * 6;
            vertices[0] = &(*va)[j];
            vertices[1] = &(*va)[j + 1];
            vertices[2] = &(*va)[j + 2];
            vertices[3] = &(*va)[j + 3];
            vertices[4] = &(*va)[j + 4];
            vertices[5] = &(*va)[j + 5];
        }

        void SetSize(float psize)
        {
            if (vertices.size() < 6)
            {
                vertices[0]->position = { 0.f, 0.f };
                return;
            }

            psize *= 0.5f;
            vertices[0]->position = { -psize, -psize };
            vertices[1]->position = { -psize, psize };
            vertices[2]->position = { psize, -psize };
            vertices[3]->position = { psize, -psize };
            vertices[4]->position = { -psize, psize };
            vertices[5]->position = { psize, psize };

            size = psize; // store this info
        }

        std::vector<sf::Vertex*> vertices;
        sf::Vector2f velocity;
        float lifeTime;
        float maxLifeTime;
        sf::Color startColor, endColor;
        bool active;
        int id;
        float size;
    };
} // namespace shak