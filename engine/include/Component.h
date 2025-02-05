#pragma once

#include "EngineDefines.h"

namespace shak
{
    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        virtual void Awake() {}
        virtual void Update(float dt) {}
        virtual void HandleInput(const sf::Event& event) {}
        virtual void OnDestroy() {}

        void SetActive(bool active) { m_active = active; }
        bool IsActive() { return m_active; }

    protected:
        bool m_active = true;
    };
}
