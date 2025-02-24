#pragma once

#include "EngineDefines.h"
#include "GameObject.h"
#include "Background.h"
#include "ShakEngine.h"

namespace shak
{
    class Camera : public GameObject
    {

    public:
        Camera(sf::FloatRect viewport, std::shared_ptr<sf::VertexArray> va = nullptr, std::shared_ptr<sf::Texture> texture = nullptr)
            : GameObject(va, texture)
            , m_bg{ nullptr }
            , m_bgSize{ 0.f, 0.f }
            , m_view{ std::make_shared<sf::View>(viewport) }
        {
            this->setOrigin(viewport.size / 2.f);
        }
        ~Camera() { std::cout << "destroyed camera" << std::endl; }

        void move(sf::Vector2f offset) override
        {
            m_view->move(offset);
            GameObject::move(offset);
        }

        void setPosition(sf::Vector2f position) override
        {
            m_view->setCenter(position);
            GameObject::setPosition(position);
        }

        std::shared_ptr<sf::View> GetView() const
        {
            return m_view;
        }

        void SetSize(const sf::Vector2f& size)
        {
            m_view->setSize(size);
            if (m_bg)
                m_bg->SetScreenSize(size);
        }

        void SetBackground(const std::shared_ptr<Background>& bg)
        {
            m_bg = bg;
        }

        // Compute size ratio at compile time
        constexpr void SetBackgroundSize(const sf::Vector2f& size)
        {
            m_bgSize = sf::Vector2f{ 1.f, 1.f }.componentWiseDiv(size);
        }

        void Awake() override
        {
            shak::ShakEngine::GetInstance().OnResize += [this](const sf::Vector2u& size)
                {
                    this->SetSize({ (float)size.x /* / m_cameras.size()*/, (float)size.y }); //TODO: find a way to have multiple cameras (try to avoid Camera ref in renderer.. if possible)
                };
        }

        void Update(float dt) override
        {
            GameObject::Update(dt);
            if (m_bg)
            {
                const sf::Vector2f camPos{ m_view->getCenter().x - m_view->getSize().x / 2, m_view->getCenter().y - m_view->getSize().y / 2 };
                const auto camSize = m_view->getSize();

                const auto sizeRatio = camSize.componentWiseMul(m_bgSize);
                const auto parallaxCamPos = camPos.componentWiseMul(sizeRatio);

                m_bg->SetScreenCoords({ parallaxCamPos, camSize });
                m_bg->setPosition(camPos);
            }
        }

        // Hack. We like the camera being a GameObj so we can set it as child of other gameObj. But we don't want the draw function to do anything, so this is an empty function.
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
        }

    private:
        std::shared_ptr<Background> m_bg;
        /// @brief Inverse of the background size. Used to compute map size ratio for parallax effect.
        sf::Vector2f m_bgSize;
        std::shared_ptr<sf::View> m_view;
    };
}

