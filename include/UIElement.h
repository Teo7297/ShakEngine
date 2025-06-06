#pragma once

//? These will be inherited so we dont have to include it in every UIElement..
#include "imgui.h"
#include "EngineDefines.h"

namespace shak
{
    // Simple virtual class that has to be extended and added to UIManager.
    class UIElement
    {
    public:
        UIElement() = default;
        virtual ~UIElement() = default;

        /// @brief This function is guaranteed to happen AFTER the awakes have been called.
        /// Use this function to get references to objects and their components.
        virtual void Init() = 0;

        /// @brief update the UIElement here. This is called before Draw().
        virtual void Update(float dt) {};

        /// @brief Implement the ImGui UI here.
        virtual void Draw() = 0;

        bool InitDone = false;

        std::string Name = "";
    };
}