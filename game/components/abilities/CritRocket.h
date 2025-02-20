#pragma once

#include "components/abilities/Ability.h"

// TODO: For the stacks feature, we should implement a buff system
class CritRocket : public Ability
{
public:
    CritRocket(AbilitySystem* abilitySystem);
    void Update(float dt) override;

private:
    // Ability Logic
    void ShootRocket(const GameObjectPtr& target);

    // Ability Events
    void SetupCallbacks();
    void RegisterCallbacks();
    std::function<void(const GameObjectPtr& target, const sf::Vector2f& pos)> ShootRocketCB;
    std::function<void(const std::shared_ptr<Ship>&)> OnTargetHit;

private:
    std::shared_ptr<sf::Texture> m_rocketTexture;
    std::shared_ptr<sf::Shader> m_rocketShader;
    float m_damage;
};