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


        void AddScene(const std::shared_ptr<Scene>& scene)
        {
            m_scenes[scene->GetName()] = scene;
        }

        void RemoveScene(const std::string& name)
        {
            m_scenes.erase(name);
        }

        std::shared_ptr<Scene> GetScene(const std::string& name)
        {
            return m_scenes[name];
        }

        // Request a scene to be activated at the end of the frame.
        void QueueScene(const std::string& name)
        {
            m_sceneToActivate = name;
        }

        // Request the next scene (in order of creation) to be activated at the end of the frame.
        void QueueNextScene()
        {
            if(m_scenes.size() < 2)
            {
                std::cout << "[SceneManager] There are no scenes in the queue. Closing application..." << std::endl;
                exit(0); // Quit with OK status anyway, used by tests
                return;
            }

            auto it = m_scenes.begin();
            while(it->second != m_activeScene && it != m_scenes.end())
            {
                ++it;
            }

            // Get the next scene in the list
            ++it;

            if(it != m_scenes.end())
            {
                m_sceneToActivate = it->first;
            }
            else
            {
                m_sceneToActivate = m_scenes.begin()->first; // Loop back to the first scene
            }
        }

        // This method force the activation of the scene. DO NOT CALL IT IN THE MIDDLE OF A FRAME.
        // It is automatically called by the engine at the end of the frame.
        void TryActivateQueuedScene()
        {
            if (m_sceneToActivate == "")
                return;

            if (m_activeScene)
                m_activeScene->InternalClear();
            m_activeScene = GetScene(m_sceneToActivate);
            m_activeScene->InternalInit();
            m_sceneToActivate = "";
        }

        std::shared_ptr<Scene> GetActiveScene()
        {
            return m_activeScene;
        }

    private:
        std::map<std::string, std::shared_ptr<Scene>> m_scenes;
        std::shared_ptr<Scene> m_activeScene;
        std::string m_sceneToActivate;
    };
}