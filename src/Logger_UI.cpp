#include "ShakUI/Logger_UI.h"

namespace shak
{
    void Logger_UI::Init()
    {
    }

    void Logger_UI::Update(float dt)
    {
        if(m_activeMessages.empty())
            return;

        UpdateMessages(dt);
    }

    void Logger_UI::Draw()
    {
        if(m_activeMessages.empty())
            return;

        ImVec2 windowSize = { ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y };
        ImGui::SetNextWindowPos(ImVec2(10.f, 10.f));
        ImGui::SetNextWindowSize(windowSize);

        ImGui::Begin("Logs", nullptr
            , ImGuiWindowFlags_NoInputs
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBackground
            | ImGuiWindowFlags_NoScrollbar
            | ImGuiWindowFlags_NoSavedSettings
        );

        ImGui::SetWindowFontScale(1.f);

        this->DrawMessages();

        ImGui::End();
    }

    void Logger_UI::LogMessage(const std::string& msg, sf::Color color, float ttl, float scale)
    {
        m_activeMessages.emplace_back(msg, color, ttl, scale, false);
    }

    int Logger_UI::LogPermanentMessage(const std::string& msg, sf::Color color, float scale)
    {
        m_activeMessages.emplace_back(msg, color, 99999.f, scale, true); // Symbolic time, it just needs to be > 0
        return m_activeMessages.size() - 1;
    }

    void Logger_UI::UpdatePermanentMessage(int id, const std::string& msg, sf::Color color, float scale)
    {
        if(id < 0 || id >= m_activeMessages.size())
            return;

        auto& message = m_activeMessages[id];
        message.msg = msg;
        message.color = color;
        message.scale = scale;
        message.isPermanent = true;
    }

    void Logger_UI::RemovePermanentMessage(int id)
    {
        if(id < 0 || id >= m_activeMessages.size())
            return;

        // set the message to be non-permanent, so it will be removed in the next update
        auto& message = m_activeMessages[id];
        message.isPermanent = false;
        message.ttl = 0.f;
    }

    void Logger_UI::ClearLogs()
    {
        m_activeMessages.clear();
    }

    void Logger_UI::UpdateMessages(float dt)
    {
        int lastActiveIndex = -1;

        for(int i = 0; i < m_activeMessages.size(); i++)
        {
            auto& msg = m_activeMessages[i];

            if(!msg.isPermanent)
                msg.ttl -= dt;

            if(msg.ttl >= 0.f)
                lastActiveIndex = i;
        }

        if(lastActiveIndex < 0)
            m_activeMessages.clear();

        else if(lastActiveIndex < m_activeMessages.size() - 1)
            m_activeMessages.erase(m_activeMessages.begin() + lastActiveIndex + 1, m_activeMessages.end());
    }

    void Logger_UI::DrawMessages()
    {
        for(auto& msg : m_activeMessages)
        {
            if(msg.ttl <= 0.f)
                continue;
            ImGui::SetWindowFontScale(msg.scale);
            ImGui::TextColored(ImVec4(msg.color.r, msg.color.g, msg.color.b, 1.f), msg.msg.c_str());
            ImGui::SetWindowFontScale(1.f);
        }
    }
}