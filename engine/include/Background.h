#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    class Background : public GameObject
    {
    public:
        Background(const std::shared_ptr<sf::Texture> texture, const sf::Vector2f& screenSize);
        ~Background() = default;

        inline void SetScreenCoords(const sf::FloatRect& coords) { m_screenCoords = coords; }
        void SetScreenSize(const sf::Vector2f& size);

        void Update(float dt) override;

    private:
        sf::FloatRect m_screenCoords;
        sf::Vector2f m_screenSize;
    };
}