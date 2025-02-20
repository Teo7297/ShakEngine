#include "BuffList.h"

BuffList::BuffList(shak::GameObject* owner)
    : shak::Component(owner)
    , m_buffs()
    , m_buffsToRemove()
{
}

BuffList::~BuffList()
{
}

void BuffList::Awake()
{
}

void BuffList::Update(float dt)
{
    // Update buffs duration
    for (auto& [name, buff] : m_buffs)
    {
        buff.timeLeft -= dt;
        if (buff.timeLeft <= 0)
        {
            buff.OnBuffEnd();
            if (buff.hasStacks)
                buff.stacks--;
            if (!buff.hasStacks || buff.stacks == 0)
                m_buffsToRemove.push_back(name);
        }
    }

    // Remove expired buffs
    for (auto& buff : m_buffsToRemove)
        this->RemoveBuff(buff);
}

void BuffList::AddBuff(const Buff& buff)
{
    if (m_buffs.contains(buff.name))
    {
        auto& buffToUpdate = m_buffs.at(buff.name);
        if (buff.hasStacks)
            buffToUpdate.stacks += buff.stacks;
        else
            buffToUpdate.duration = buff.duration;
    }
    else
        m_buffs[buff.name] = buff;
}

void BuffList::RemoveBuff(const std::string& name)
{
    if (m_buffs.contains(name))
        m_buffs.erase(name);
}

int BuffList::GetBuffStacks(const std::string& name) const
{
    if (m_buffs.contains(name))
        return m_buffs.at(name).stacks;
    else
        return 0;
}


