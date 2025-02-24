#pragma once

#include "UIManager.h"
#include "UIElement.h"

#include "components/Health.h"

class HUD : public shak::UIElement
{
public:
    HUD() = default;
    ~HUD() override = default;

    void Init() override
    {
        m_playerHealth = shak::ShakEngine::GetInstance().FindGameObjectByName("Player")->GetComponent<Health>();
        m_hp = m_playerHealth->GetHealth();
        m_totalHp = m_playerHealth->GetMaxHealth();
        m_playerHealth->OnHealthChange += [this](float hp, float totalHp)
            {
                m_hp = hp;
                m_totalHp = totalHp;
            };
    }

    void Draw() override
    {
        // Before creating the window, tell ImGui where you want it:
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        // Push style colors for transparent background and borders
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));  // Fully transparent background
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));  // Fully transparent borders

        ImGui::Begin("HUD Overlay", nullptr
            , ImGuiWindowFlags_NoInputs
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBackground
        );

        // Top left region (for example, player stats)
        ImGui::BeginChild("TopLeft", ImVec2(300, 150), false);
        std::stringstream ss;
        ss << "Health: ";
        std::string hpTxt = std::to_string(m_hp);
        ss << hpTxt.substr(0, hpTxt.find('.'));
        ss << " / ";
        std::string tothpTxt = std::to_string(m_totalHp);
        ss << tothpTxt.substr(0, tothpTxt.find('.'));
        ImGui::Text(ss.str().c_str());
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("This is the tooltip for the health text");
            ImGui::EndTooltip();
        }
        ImGui::Text("Energy: 100");
        // Add more widgets...
        ImGui::EndChild();

        // Top right region (for example, minimap)
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 310); // 300 width + margin
        ImGui::BeginChild("TopRight", ImVec2(300, 300), false);
        ImGui::Text("Minimap");
        // Add minimap widget...
        ImGui::EndChild();

        // Bottom left region (inventory, etc.)
        ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 150);
        ImGui::BeginChild("BottomLeft", ImVec2(400, 150), false);
        ImGui::Text("Inventory");
        // Add inventory widgets...
        if (ImGui::Button("Click Me"))
            std::cout << "Clicked!" << std::endl;
        ImGui::EndChild();

        ImGui::End();
        ImGui::PopStyleColor(2);
    }

private:
    std::shared_ptr<Health> m_playerHealth;
    float m_hp = 0.f;
    float m_totalHp = 0.f;
};