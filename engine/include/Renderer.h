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
        void CloseWindow();

        void AddCamera(const std::string& name, std::shared_ptr<sf::View> camera);
        void RemoveCamera(const std::string& name);

        inline void SetClearColor(sf::Color color) { m_clearColor = color; }

        void Render(const std::vector<GameObjectPtr>& drawables);
    private:
        void Draw(const std::vector<GameObjectPtr>& drawables);

    private:
        bool m_isRunning = true;
        std::shared_ptr<sf::RenderWindow> m_window = nullptr;
        std::thread m_renderThread;
        std::unordered_map<std::string, std::shared_ptr<sf::View>> m_cameras;
        sf::Color m_clearColor = { 2, 38, 46 };
    };
}
