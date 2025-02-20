#pragma once

#include "ShakEngine.h"
#include "Component.h"
#include "ShakEvent.h"

namespace shak
{
    class Sprite;
}
class Energy;
class EnergyBar : public shak::Component
{
public:
    EnergyBar(shak::GameObject* owner);
    ~EnergyBar() override;

    void Awake() override;

private:
    std::function<void(float, float)> m_onEnergyChange;

private:
    std::shared_ptr<Energy> m_energy;
    std::shared_ptr<shak::Sprite> m_frontUI;
    std::shared_ptr<shak::Sprite> m_backgroundUI;
    sf::Vector2f m_size;
};