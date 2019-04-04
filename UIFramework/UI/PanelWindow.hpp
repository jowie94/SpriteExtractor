#pragma once

#include "BaseWindow.hpp"

#include <ImGui/imgui.h>

class PanelWindow : public BaseWindow
{
public:
    enum class Position
    {
        Left,
        Middle,
        Right,
        Up,
        Down
    };

    PanelWindow(const char* name, const ImVec2& initialSize, Position position, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

    Position GetPosition() const
    {
        return _position;
    }
    
private:
    bool BeginWidget() override final;
    void EndWidget(bool wasDrawn) override final;

    ImVec2 _initialSize;
    Position _position;
};
