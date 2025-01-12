#include "Renderer.h"

namespace shak
{
    Renderer::Renderer()
    {
    }

    Renderer::~Renderer()
    {
    }

    std::shared_ptr<sf::RenderWindow> Renderer::CreateSFWindow(const std::string& title)
    {
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT }), title);
        return m_window;
    }

    void Renderer::PushToRenderQueue(const std::vector<shak::Drawable>& drawables)
    {
        std::lock_guard<std::mutex> lock(m_renderQueueMutex);
        for (const auto& drawable : drawables)
            m_renderQueue.push(drawable);
        m_stopCondition.notify_one(); // Notify the rendering thread
    }

    void Renderer::ClearRenderQueue()
    {
        std::lock_guard<std::mutex> lock(m_renderQueueMutex);
        while (!m_renderQueue.empty())
            m_renderQueue.pop();
        m_stopCondition.notify_one(); // Notify the rendering thread
    }

    void Renderer::Start()
    {
        if (!m_window)
            std::cerr << "[Renderer - Main Thread] No window to render to" << std::endl;

        // deactivate its OpenGL context in this thread, we are going to enable it in the render-thread
        if (!m_window->setActive(false))
            std::cerr << "[Renderer - Main Thread] OpenGL context could not be deactivated" << std::endl;
        m_renderThread = std::thread(&Renderer::RenderLoop, this);
    }

    void Renderer::Stop()
    {
        ClearRenderQueue();
        m_isRunning = false;
        m_stopCondition.notify_all();
        m_renderThread.join();
    }

    void Renderer::RenderLoop()
    {
        // activate the window's context
        if (!m_window->setActive(true))
            std::cerr << "[Render thread] OpenGL context could not be activated" << std::endl;


        // the rendering loop
        while (m_isRunning)
        {
            std::unique_lock<std::mutex> lock(m_renderQueueMutex);

            // Wait for items to draw or for the application to close
            m_stopCondition.wait(lock, [this] { return !m_renderQueue.empty() || !m_isRunning; });

            m_window->clear({ 2, 38, 46 });
            while (!m_renderQueue.empty())
            {
                const auto [drawable, renderStates] = m_renderQueue.front();
                m_renderQueue.pop();
                if (!renderStates)
                    m_window->draw(*drawable);
                else
                    m_window->draw(*drawable, *renderStates);
            }

            m_window->display();
        }

        // The render thread has the authority over the window, we could also deactivate it here and close it from the main thread I guess...
        m_window->close();
    }
}