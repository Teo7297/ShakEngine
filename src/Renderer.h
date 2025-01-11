#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        std::shared_ptr<sf::RenderWindow> CreateSFWindow(const std::string& title);
        void PushToRenderQueue(const std::vector<shak::Drawable>& drawables);
        void ClearRenderQueue();
        void Start();
        void Stop();

    private:
        void RenderLoop();

    private:
        std::queue<shak::Drawable> m_renderQueue;
        std::mutex m_renderQueueMutex;
        std::condition_variable m_stopCondition;
        bool m_isRunning = true;
        std::shared_ptr<sf::RenderWindow> m_window = nullptr;
        std::thread m_renderThread;

    };
}
