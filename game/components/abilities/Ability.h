#pragma once

#include "ShakEngine.h"
#include "ShakEvent.h"

class AbilitySystem;
class Ability
{
protected:
    enum class Type
    {
        Active,
        Passive
    };

public:
    Ability(const std::string& name, float cooldown, float duration, Type type, AbilitySystem* abilitySystem)
        : m_name(name)
        , m_cooldown(cooldown)
        , m_currentCooldown(0.0f)
        , m_duration(duration)
        , m_currentDuration(0.0f)
        , m_isActive(type == Type::Passive)
        , m_type(type)
        , m_abilitySystem(abilitySystem)
    {
    }

    bool Toggle(const GameObjectPtr& target = nullptr, const sf::Vector2f& targetPos = { 0, 0 })
    {
        if (m_isActive)
        {
            Deactivate();
            return false;
        }
        else
        {
            Activate(target, targetPos);
            return true;
        }
    }

    void Activate(const GameObjectPtr& target = nullptr, const sf::Vector2f& targetPos = { 0, 0 })
    {
        if (m_isActive || m_currentCooldown > 0.0f)
            return;

        if (m_duration > 0.f)
        {
            m_isActive = true;
            m_currentDuration = m_duration;
        }

        m_currentCooldown = m_cooldown;
        OnActivated(target, targetPos);
    }

    void Deactivate()
    {
        if (!m_isActive)
            return;

        m_isActive = false;
        OnDeactivated();
    }

    virtual void Update(float dt)
    {
        if (m_type == Type::Active && m_currentCooldown > 0.0f)
        {
            m_currentCooldown -= dt;
            if (m_currentCooldown <= 0.0f)
            {
                m_currentCooldown = 0.0f;
                OnCooldown();
            }
        }

        if (m_type == Type::Active && m_isActive && m_currentDuration > 0.f)
        {
            m_currentDuration -= dt;
            if (m_currentDuration <= 0.f)
            {
                m_currentDuration = 0.0f;
                Deactivate();
            }
        }
    }

public:
    // EVENTS
    shak::Event<> OnCooldown;
    shak::Event<const GameObjectPtr&, const sf::Vector2f&> OnActivated;
    shak::Event<> OnDeactivated;

protected:
    std::string m_name;
    bool m_isActive;
    float m_cooldown, m_currentCooldown;
    float m_duration, m_currentDuration;
    Type m_type;
    AbilitySystem* m_abilitySystem;
};