#pragma once

#include "UIElement.h"
#include "EngineDefines.h"

namespace shak
{
    struct LogMessage
    {
        std::string msg;
        sf::Color color;
        float ttl;
        float scale;
    };

    class Logger_UI : public UIElement
    {
    public:
        Logger_UI() = default;
        ~Logger_UI() override = default;

        void Init() override;
        void Update(float dt) override;
        void Draw() override;

        void LogMessage(const std::string& msg, sf::Color color = sf::Color::Green, float ttl = 3.f, float scale = 1.f);
        void ClearLogs();

    private:
        void UpdateMessages(float dt);
        void DrawMessages();

    private:
        std::vector<::shak::LogMessage> m_activeMessages;
    };
}