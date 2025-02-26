#pragma once

#include <type_traits>

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    template<typename T>
    class GameObjectPool
    {
    public:
        GameObjectPool(size_t size = 10ull)
            : m_pool()
        {
            m_pool.reserve(size);
            static_assert(std::is_base_of<GameObject, T>::value, "GameObjectPool instantiated with class not derived from GameObject");
        }
        ~GameObjectPool() {}

        template <typename ... Args>
        std::shared_ptr<T> Get(Args&& ... args)
        {
            for (auto& obj : m_pool)
            {
                if (!obj->IsActive())
                {
                    obj->SetActive(true);
                    return obj;
                }
            }

            // No free objects found, create a new one
            return Make(std::forward<Args>(args)...);
        }

        int GetActiveCount() const
        {
            int count = 0;
            for (const auto& obj : m_pool)
            {
                if (obj->IsActive())
                    count++;
            }
            return count;
        }

        int GetInactiveCount() const
        {
            return m_pool.size() - GetActiveCount();
        }

        int GetTotalCount() const
        {
            return m_pool.size();
        }

    private:
        template <typename ... Args>
        std::shared_ptr<T> Make(Args&& ... args)
        {
            auto obj = std::make_shared<T>(std::forward<Args>(args)...);
            m_pool.push_back(obj);
            return obj;
        }

    private:
        std::vector<std::shared_ptr<T>> m_pool;
    };
}