#pragma once

#include "EngineDefines.h"

namespace shak
{
    class GameObject;
    class Component
    {
    public:
        Component(GameObject* owner)
            : m_active(true)
            , m_needAwake(true)
            , m_owner(owner)
        {
        }
        virtual ~Component() = default;

        virtual void Awake() { m_needAwake = false; }
        virtual void Update(float dt) {}
        virtual void HandleInput(const sf::Event& event) {}
        virtual void OnDestroy() {}

        void SetActive(bool active) { m_active = active; }
        bool IsActive() { return m_active; }
        bool NeedAwake() { return m_needAwake; }
        GameObject* GetOwner() { return m_owner; }

    protected:
        bool m_active;
        bool m_needAwake;
        GameObject* m_owner;
    };
}
