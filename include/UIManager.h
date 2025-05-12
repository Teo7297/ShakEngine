#pragma once

#include "EngineDefines.h"
#include "UIElement.h"

namespace shak
{
    /// @brief UIManager. Simply add UIElement(s) here and select the active one.
    // This design relies on having a single active UIElement at a time, BUT you can add multiple children to it
    // and manage them as you want in the virtual Draw function.
    class UIManager
    {
    public:
        UIManager() = default;
        ~UIManager() = default;

        void AddUIElement(const std::string& name, const std::shared_ptr<UIElement>& element)
        {
            element->Name = name;
            m_elements[name] = element;
        }

        void RemoveUIElement(const std::string& name)
        {
            m_elements.erase(name);
        }

        std::shared_ptr<UIElement> GetUIElement(const std::string& name)
        {
            return m_elements[name];
        }

        void SelectActiveUI(const std::string& name)
        {
            m_activeUIs.emplace_back(GetUIElement(name));
        }

        void DeselectActiveUI(const std::string& name)
        {
            auto it = std::remove_if(m_activeUIs.begin(), m_activeUIs.end(),
                [&name](const std::shared_ptr<UIElement>& ui) { return ui->Name == name; });
            m_activeUIs.erase(it, m_activeUIs.end());
            m_activeUIs.shrink_to_fit();
        }

        std::vector<std::shared_ptr<UIElement>> GetActiveUIs()
        {
            return m_activeUIs;
        }

        void UpdateUI(float dt)
        {
            for(auto& ui : m_activeUIs)
                if(ui->InitDone)
                    ui->Update(dt);
        }

        void DrawUI()
        {
            for(auto& ui : m_activeUIs)
                if(ui->InitDone)
                    ui->Draw();
        }

    private:
        // name, element
        std::unordered_map<std::string, std::shared_ptr<UIElement>> m_elements;
        std::vector<std::shared_ptr<UIElement>> m_activeUIs;
    };
}