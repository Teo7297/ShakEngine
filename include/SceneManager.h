#pragma once

#include "EngineDefines.h"
#include "Scene.h"

namespace shak
{
    class SceneManager
    {
    public:
        SceneManager() = default;
        ~SceneManager() = default;


        void AddScene(const std::string& name, const std::shared_ptr<Scene>& scene)
        {
            m_scenes[name] = scene;
        }

        void RemoveScene(const std::string& name)
        {
            m_scenes.erase(name);
        }

        std::shared_ptr<Scene> GetScene(const std::string& name)
        {
            return m_scenes[name];
        }

        void ActivateScene(const std::string& name)
        {
            if (m_activeScene)
                m_activeScene->InternalClear();
            m_activeScene = GetScene(name);
            m_activeScene->InternalInit();
        }

        std::shared_ptr<Scene> GetActiveScene()
        {
            return m_activeScene;
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
        std::shared_ptr<Scene> m_activeScene;
    };
}