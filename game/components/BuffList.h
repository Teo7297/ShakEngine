#pragma once

#include "ShakEngine.h"
#include "Component.h"
#include "ShakEvent.h"

struct Buff
{
    std::string name;
    float duration;
    float timeLeft;
    int stacks;
    bool hasStacks;
    shak::Event<> OnBuffEnd;

    bool operator==(const Buff& other) const
    {
        return name == other.name;
    }
};

class BuffList : public shak::Component
{
public:
    BuffList(shak::GameObject* owner);
    ~BuffList() override;

    void Awake() override;
    void Update(float dt) override;

    void AddBuff(const Buff& buff);
    void RemoveBuff(const std::string& name);
    int GetBuffStacks(const std::string& name) const;

private:
    std::unordered_map<std::string, Buff> m_buffs;
    std::vector<std::string> m_buffsToRemove;
};