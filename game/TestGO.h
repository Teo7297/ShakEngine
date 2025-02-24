#pragma once

#include "GameObject.h"
#include "ParticleSystem.h"

class TestGO : public shak::Component
{
public:
    TestGO(shak::GameObject* owner)
        : shak::Component(owner)
    {
    }
    ~TestGO() override = default;

    void Awake() override
    {

    }

    void Update(float dt) override {};
    void HandleInput(const sf::Event& event) override
    {
        if (auto key = event.getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::E)
            {
                std::cout << "E pressed on " << m_owner->Name << std::endl;
                auto cid = m_owner->Id;
                // auto cid = m_owner->GetChildren()[0]->Id;
                shak::ShakEngine::GetInstance().GetScene()->RemoveGameObject(cid);
            }

            if (key->code == sf::Keyboard::Key::Q)
            {
                for (const auto& child : m_owner->GetChildren())
                {
                    std::cout << "Child: " << child->Name << std::endl;
                }
            }
        }
    }
    void OnDestroy() override {};
};