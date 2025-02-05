#pragma once

#include "EngineDefines.h"

namespace shak
{
    template<typename Output, typename ... Input>
    class Event
    {
    public:
        void Add(std::function<Output(Input ...)> f)
        {
            m_callbacks.emplace_back(f);
            m_attached++;
        }

        void Remove(std::function<Output(Input ...)> f)
        {
            auto found = std::remove(m_callbacks.begin(), m_callbacks.end(), f);
            if (found != m_callbacks.end())
            {
                m_callbacks.erase(found);
                m_attached--;
            }
        }

        int GetAttachedCount() const
        {
            return m_attached;
        }

        void operator()(Input ... args)
        {
            for (const auto& f : m_callbacks)
                f(std::forward<Input>(args)...);
        }

        explicit operator bool() const
        {
            return m_attached > 0;
        }

    private:
        std::vector<std::function<Output(Input ...)>> m_callbacks;
        int m_attached = 0;
    };
}