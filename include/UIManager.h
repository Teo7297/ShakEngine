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
            m_activeUI = GetUIElement(name);
        }

        void DeselectActiveUI()
        {
            m_activeUI = nullptr;
        }

        std::shared_ptr<UIElement> GetActiveUI()
        {
            return m_activeUI;
        }

        void UpdateUI(float dt)
        {
            if (m_activeUI)
            {
                m_activeUI->Update(dt);
            }
        }

        void DrawUI()
        {
            if (m_activeUI)
            {
                m_activeUI->Draw();
            }
        }

    private:
        // name, element
        std::unordered_map<std::string, std::shared_ptr<UIElement>> m_elements;
        std::shared_ptr<UIElement> m_activeUI;
    };
}