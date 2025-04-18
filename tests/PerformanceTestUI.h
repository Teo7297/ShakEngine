#pragma once

#include "UIElement.h"
#include <vector>
#include <algorithm>
#include "imgui.h"

class PerformanceTestUI : public shak::UIElement
{
private:
    static const int FPS_HISTORY_SIZE = 100; // Number of frames to keep in history
    std::vector<float> fpsHistory;
    int fpsHistoryOffset = 0;

    // Updated static helper function that doesn't rely on UserData
    static float GetFpsValue(void* data, int idx)
    {
        PerformanceTestUI* instance = static_cast<PerformanceTestUI*>(data);
        int historyPos = (instance->fpsHistoryOffset + idx) % FPS_HISTORY_SIZE;
        return instance->fpsHistory[historyPos];
    }

public:
    void Init() override
    {
        // Initialize the FPS history buffer with zeros
        fpsHistory.resize(FPS_HISTORY_SIZE, 0.0f);
    }

    void Update(float dt) override
    {
        // Add current FPS to history
        float currentFps = ImGui::GetIO().Framerate;
        fpsHistoryOffset = (fpsHistoryOffset + 1) % FPS_HISTORY_SIZE;
        fpsHistory[fpsHistoryOffset] = currentFps;
    }

    void Draw() override
    {
        // add a imgui window with fps counter
        ImGui::Begin("Performance Test UI");
        // Also add a graph to show the fps over time
        ImGui::Text("Performance Test UI");
        
        float currentFps = ImGui::GetIO().Framerate;
        ImGui::Text("FPS: %.1f", currentFps);
        ImGui::Text("Frame Time: %.1f ms", 1000.f / currentFps);
        
        // Calculate min and max values for better graph scaling
        float minFps = *std::min_element(fpsHistory.begin(), fpsHistory.end());
        float maxFps = *std::max_element(fpsHistory.begin(), fpsHistory.end());
        // Add some padding to the range
        minFps = std::max(0.0f, minFps - 5.0f);
        maxFps = maxFps + 5.0f;
        
        // Draw FPS graph - passing 'this' directly as the data pointer
        ImGui::PlotLines("FPS History", 
                         &GetFpsValue, 
                         this, 
                         FPS_HISTORY_SIZE, 
                         0, 
                         NULL, 
                         minFps, 
                         maxFps, 
                         ImVec2(0, 80));
        
        ImGui::End();
    }
};