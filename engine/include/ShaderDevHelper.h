#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

class ShaderDevHelper : public shak::GameObject
{
public:
    ShaderDevHelper(std::string shaderName)
        : shak::GameObject{},
        vs{ "assets/shaders/" + shaderName + ".vs" },
        fs{ "assets/shaders/" + shaderName + ".fs" }
    {
        m_vertices = std::make_shared<sf::VertexArray>(sf::PrimitiveType::TriangleStrip, 4);
        (*m_vertices)[0].position = sf::Vector2f(0.f, 0.f);
        (*m_vertices)[1].position = sf::Vector2f(1920.f, 0.f);
        (*m_vertices)[2].position = sf::Vector2f(0.f, 1080.f);
        (*m_vertices)[3].position = sf::Vector2f(1920.f, 1080.f);
    }
    ~ShaderDevHelper() {}

    void Update(float dt) override
    {
        m_time += dt;
        auto newShader = std::make_shared<sf::Shader>();
        if (newShader->loadFromFile("assets/shaders/particle.vs", "assets/shaders/particle.fs"))
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

