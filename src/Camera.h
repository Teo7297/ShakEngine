#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    class Camera : public shak::GameObject
    {

    public:
        Camera(sf::FloatRect viewport, std::shared_ptr<sf::VertexArray> va = nullptr, std::shared_ptr<sf::Texture> texture = nullptr) : GameObject(va, texture), m_view{ std::make_shared<sf::View>(viewport) } {}
        ~Camera() { std::cout << "destroyed camera" << std::endl; }

        void move(sf::Vector2f offset) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_view->setCenter(m_view->getCenter() + offset);
        }

        std::shared_ptr<sf::View> GetView() const
        {
            return m_view;
        }

        // Hack. We like the camera being a GameObj so we can set it as child of other gameObj. But we don't want the draw function to do anything, so this is an empty function.
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
        }

    private:
        std::shared_ptr<sf::View> m_view;
    };
}

