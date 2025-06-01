#pragma once

#include "GameObject.h"
#include "ShakUI/Logger_UI.h"
#include "ShakEngine.h"
#include "shapes/Square.h"

namespace shak
{
    class RectCollider : public shak::Square
    {
    public:
        RectCollider(const sf::FloatRect& bounds)
            : Square(bounds.size, sf::Color::Yellow, Type::Outlined, false)
            , m_bounds{ bounds }
            , m_drawDebug{ false }
        {
            this->EnablePhysics();
        }

        ~RectCollider() override
        {
        }

        void Awake() override
        {
            if(m_parent->Name == "Root")
            {
                m_engine->GetLogger()->LogErrorMessage("RectCollider does not have a proper parent");
            }

            this->setPosition(m_bounds.position);
        }

        void Update(float dt) override
        {
        }

        void OnCollision(const std::shared_ptr<shak::GameObject>& other) override
        {
            m_parent->OnCollision(other); // Just invoke the collision on the parent object
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
        {
            if(!m_drawDebug)
                return;
            else
                Square::draw(target, states);
        }

        void ToggleDrawDebug(bool active)
        {
            m_drawDebug = active;
        }

    private:
        sf::FloatRect m_bounds;
        bool m_drawDebug;
    };
}