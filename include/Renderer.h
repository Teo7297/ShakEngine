#pragma once

#include "EngineDefines.h"

namespace shak
{
    class GameObject;
    class Camera;
    enum class CameraResizeBehavior;
    class Renderer
    {
    public:
        Renderer();
        ~Renderer() = default;

        std::shared_ptr<sf::RenderWindow> CreateSFWindow(const std::string& title);
        void CloseWindow();

        std::shared_ptr<Camera> AddCamera(const std::string& name, const sf::FloatRect& viewport, const CameraResizeBehavior& resizeBehavior);
        void RemoveCamera(const std::string& name);

        void SetClearColor(sf::Color color) { m_clearColor = color; }

        void Render(const std::vector<std::shared_ptr<GameObject>>& drawables);

        void ToggleBlending(bool enable)
        {
            m_blendingEnabled = enable;
        }

    private:
        bool m_isRunning;
        std::shared_ptr<sf::RenderWindow> m_window;
        std::vector<std::shared_ptr<shak::Camera>> m_cameras;
        sf::Color m_clearColor;
        bool m_blendingEnabled;
    };
}
