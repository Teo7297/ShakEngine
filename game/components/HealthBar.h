#pragma once

#include "ShakEngine.h"
#include "Component.h"
#include "ShakEvent.h"

namespace shak
{
    class Sprite;
}
class Health;
class HealthBar : public shak::Component
{
public:
    HealthBar(shak::GameObject* owner);
    ~HealthBar() override;

    void Awake() override;
    void LateUpdate(float dt) override;

private:
    std::function<void(float, float)> m_onHealthChange;

private:
    std::shared_ptr<Health> m_health;
    std::shared_ptr<shak::Sprite> m_hpFrontUI;
    std::shared_ptr<shak::Sprite> m_hpBackgroundUI;
    sf::Vector2f m_size;
};