#pragma once

#include "EngineDefines.h"

namespace shak
{
    template<typename ... Input>
    class Event
    {
    public:
        Event()
            : m_callbacks()
            , m_attached(0)
        {
        }
        ~Event() = default;

        void Add(std::function<void(Input ...)> f)
        {
            m_callbacks.emplace_back(f);
            m_attached++;
        }

        void operator+=(std::function<void(Input ...)> f)
        {
            Add(f);
        }

        void Reset()
        {
            m_callbacks.clear();
            m_attached = 0;
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


    private:
        std::vector<std::function<void(Input ...)>> m_callbacks;
        int m_attached;
    };
}