#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    class Background : public GameObject
    {
    public:
        Background(std::shared_ptr<sf::Texture> texture, sf::Vector2f screenSize);
        ~Background();

        void SetScreenCoords(sf::FloatRect coords) { m_screenCoords = coords; }

        void Update(float dt) override;
    private:
        sf::FloatRect m_screenCoords;
        sf::Vector2f m_screenSize;
    };
}