#include "ShakUI/Logger_UI.h"

namespace shak
{
    void Logger_UI::Init()
    {
    }

    void Logger_UI::Update(float dt)
    {
        UpdateMessages(dt);
    }

    void Logger_UI::Draw()
    {
        using namespace ImGui;

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

        End();
    }

    void Logger_UI::LogMessage(const std::string& msg, sf::Color color, float ttl, float scale)
    {
        m_activeMessages.emplace_back(msg, color, ttl, scale);
    }

    void Logger_UI::ClearLogs()
    {
        m_activeMessages.clear();
    }

    void Logger_UI::UpdateMessages(float dt)
    {
        // eh just remove one at a time if multiple messages expire together
        int toRemove = -1;

        for(int i = 0; i < m_activeMessages.size(); i++)
        {
            auto& msg = m_activeMessages[i];
            msg.ttl -= dt;
            if(msg.ttl <= 0.f)
                toRemove = i;
        }

        if(toRemove >= 0)
            m_activeMessages.erase(m_activeMessages.begin() + toRemove);
    }

    void Logger_UI::DrawMessages()
    {
        for(auto& msg : m_activeMessages)
        {
            ImGui::SetWindowFontScale(msg.scale);
            ImGui::TextColored(ImVec4(msg.color.r, msg.color.g, msg.color.b, 1.f), msg.msg.c_str());
            ImGui::SetWindowFontScale(1.f);
        }
    }
}