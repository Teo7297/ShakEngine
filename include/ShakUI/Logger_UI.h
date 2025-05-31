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
        bool isPermanent;
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
        int LogPermanentMessage(const std::string& msg, sf::Color color = sf::Color::Green, float scale = 1.f);
        void UpdatePermanentMessage(int id, const std::string& msg, sf::Color color = sf::Color::Green, float scale = 1.f);
        void RemovePermanentMessage(int id);
        void ClearLogs();

    private:
        void UpdateMessages(float dt);

        // Messages are drawn in reverse order, so we can have the new message pop on top
        void DrawMessages();

    private:
        std::vector<::shak::LogMessage> m_activeMessages;
    };
}