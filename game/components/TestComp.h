#pragma once

#include "ShakEngine.h"
#include "Component.h"

class TestComp : public shak::Component
{
public:
    TestComp(shak::GameObject* owner)
        : shak::Component(owner)
    {
    }

    void Awake() override
    {
        std::cout << "Awake called!" << std::endl;
    }
};