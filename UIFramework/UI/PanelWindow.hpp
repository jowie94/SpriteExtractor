#pragma once

#include "BaseWindow.hpp"

#include <ImGui/imgui.h>

class PanelWindow : public BaseWindow
{
public:
    enum class Position
    {
        None,
        Left,
        Middle,
        Right,
        Up,
        Down
    };

    PanelWindow(const char* name, const ImVec2& initialSize, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

private:
    bool BeginWidget() override final;
    void EndWidget(bool wasDrawn) override final;

    ImVec2 _initialSize;
};
