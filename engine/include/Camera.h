#pragma once

#include "EngineDefines.h"
#include "GameObject.h"
#include "Background.h"

namespace shak
{
    class Camera : public GameObject
    {

    public:
        Camera(sf::FloatRect viewport, std::shared_ptr<sf::VertexArray> va = nullptr, std::shared_ptr<sf::Texture> texture = nullptr) : GameObject(va, texture), m_view{ std::make_shared<sf::View>(viewport) }
        {
        }
        ~Camera() { std::cout << "destroyed camera" << std::endl; }

        void move(sf::Vector2f offset) override
        {
            m_view->move(offset);
        }

        std::shared_ptr<sf::View> GetView() const
        {
            return m_view;
        }

        void SetSize(const sf::Vector2f& size)
        {
            m_view->setSize(size);
            m_bg->SetScreenSize(size);
        }

        inline void SetBackground(const std::shared_ptr<Background>& bg) { m_bg = bg; }

        void Update(float dt) override
        {
            GameObject::Update(dt);
            if (m_bg)
            {
                sf::Vector2f camPos = { m_view->getCenter().x - m_view->getSize().x / 2, m_view->getCenter().y - m_view->getSize().y / 2 };
                auto camSize = m_view->getSize();
                m_bg->SetScreenCoords({ camPos, camSize });
                m_bg->setPosition(camPos);
            }
        }

        // Hack. We like the camera being a GameObj so we can set it as child of other gameObj. But we don't want the draw function to do anything, so this is an empty function.
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
        }

    private:
        std::shared_ptr<Background> m_bg;
        std::shared_ptr<sf::View> m_view;
    };
}

