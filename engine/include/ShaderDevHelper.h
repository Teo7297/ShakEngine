#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

class ShaderDevHelper : public shak::GameObject
{
public:
    ShaderDevHelper(std::string shaderName, std::shared_ptr<sf::Texture> texture = nullptr)
        : shak::GameObject{},
        vs{ "assets/shaders/" + shaderName + ".vs" },
        fs{ "assets/shaders/" + shaderName + ".fs" }
    {
        m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
        (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
        (*m_vertices)[1].position = sf::Vector2f(0.f, 1080.f);
        (*m_vertices)[2].position = sf::Vector2f(1920.f, 0.f);
        (*m_vertices)[3].position = sf::Vector2f(1920.f, 1080.f);

        (*m_vertices)[0].color = sf::Color::White;
        (*m_vertices)[1].color = sf::Color::White;
        (*m_vertices)[2].color = sf::Color::White;
        (*m_vertices)[3].color = sf::Color::White;

        // tex
        m_texture = texture;
        auto size = texture->getSize();
        (*m_vertices)[0].texCoords = sf::Vector2f(0.f, 0.f);
        (*m_vertices)[1].texCoords = sf::Vector2f(0.f, size.y);
        (*m_vertices)[2].texCoords = sf::Vector2f(size.x, 0.f);
        (*m_vertices)[3].texCoords = sf::Vector2f(size.x, size.y);
    }
    ~ShaderDevHelper() {}

    void Update(float dt) override
    {
        m_time += dt;
        auto newShader = std::make_shared<sf::Shader>();
        if (newShader->loadFromFile("assets/shaders/laserShot.fs", sf::Shader::Type::Fragment))
        {
            m_shader = newShader;
            m_shader->setUniform("u_texture", *m_texture);
            m_shader->setUniform("u_resolution", sf::Glsl::Vec2{ 1920, 1080 });
        }
        m_shader->setUniform("u_time", m_time);
    }

private:
    fs::path vs, fs;
    float m_time = 0.f;
};

