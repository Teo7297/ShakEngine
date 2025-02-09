#pragma once

#include "ShakEngine.h"
#include "ShakEvent.h"
class Ability
{
protected:
    enum class Type
    {
        Active,
        Passive
    };

public:
    Ability(const std::string& name, float cooldown, float duration, Type type)
        : m_name(name)
        , m_cooldown(cooldown)
        , m_currentCooldown(0.0f)
        , m_duration(duration)
        , m_currentDuration(0.0f)
        , m_isActive(type == Type::Passive)
        , m_type(type)
    {
    }

    void Activate()
    {
        if (m_isActive || m_currentCooldown > 0.0f)
            return;

        if (m_duration > 0.f)
        {
            m_isActive = true;
            m_currentDuration = m_duration;
        }

        m_currentCooldown = m_cooldown;
        OnActivated();
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
    shak::Event<> OnActivated;
    shak::Event<> OnDeactivated;

protected:
    std::string m_name;
    bool m_isActive;
    float m_cooldown, m_currentCooldown;
    float m_duration, m_currentDuration;
    Type m_type;

};