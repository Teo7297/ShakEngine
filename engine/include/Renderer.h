#pragma once

#include "EngineDefines.h"

namespace shak
{
    class GameObject;
    class Camera;
    class Renderer
    {
    public:
        Renderer();
        ~Renderer() = default;

        std::shared_ptr<sf::RenderWindow> CreateSFWindow(const std::string& title);
        void CloseWindow();

        std::shared_ptr<shak::Camera> AddCamera(const std::string& name, const sf::FloatRect& viewport);
        void RemoveCamera(const std::string& name);

        void SetClearColor(sf::Color color) { m_clearColor = color; }

        void Render(const std::vector<std::shared_ptr<GameObject>>& drawables);
    private:
        void Draw(const std::vector<std::shared_ptr<GameObject>>& drawables);

    private:
        bool m_isRunning;
        std::shared_ptr<sf::RenderWindow> m_window;
        std::unordered_map<std::string, std::shared_ptr<shak::Camera>> m_cameras;
        sf::Color m_clearColor;
    };
}
