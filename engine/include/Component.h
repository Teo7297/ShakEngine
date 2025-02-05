#pragma once

#include "EngineDefines.h"

namespace shak
{
    class GameObject;
    class Component
    {
    public:
        Component(GameObject* owner);
        virtual ~Component() = default;

        virtual void Awake() {}
        virtual void Update(float dt) {}
        virtual void HandleInput(const sf::Event& event) {}
        virtual void OnDestroy() {}

        // template<typename T>
        // std::shared_ptr<T> GetComponent()
        // {
        //     return m_owner->GetComponent<T>();
        // }

        void SetActive(bool active) { m_active = active; }
        bool IsActive() { return m_active; }

    protected:
        bool m_active = true;
        GameObject* m_owner = nullptr;
    };
}
