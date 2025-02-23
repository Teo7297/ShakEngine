#pragma once

#include "EngineDefines.h"
#include "GameObject.h"

namespace shak
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer() = default;

        std::shared_ptr<sf::RenderWindow> CreateSFWindow(const std::string& title);
        void CloseWindow();

        void AddCamera(const std::string& name, std::shared_ptr<sf::View> camera);
        void RemoveCamera(const std::string& name);

        void SetClearColor(sf::Color color) { m_clearColor = color; }

        void Render(const std::vector<GameObjectPtr>& drawables);
    private:
        void Draw(const std::vector<GameObjectPtr>& drawables);

    private:
        bool m_isRunning;
        std::shared_ptr<sf::RenderWindow> m_window;
        std::unordered_map<std::string, std::shared_ptr<sf::View>> m_cameras;
        sf::Color m_clearColor;
    };
}
