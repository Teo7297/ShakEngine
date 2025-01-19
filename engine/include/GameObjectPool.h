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
        GameObjectPool()
        {
            static_assert(std::is_base_of<GameObject, T>::value, "GameObjectPool instantiated with class not derived from GameObject");
        }
        ~GameObjectPool() {}

        std::shared_ptr<T> Get()
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
            return Make();
        }

    private:
        std::shared_ptr<T> Make()
        {
            auto obj = std::make_shared<T>();
            m_pool.push_back(obj);
            return obj;
        }

    private:
        std::vector<std::shared_ptr<T>> m_pool;
    };
}