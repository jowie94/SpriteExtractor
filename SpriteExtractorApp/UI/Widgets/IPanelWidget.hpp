#pragma once

#include "IWidget.hpp"

#include <ImGui/imgui.h>

class IPanelWidget : public IWidget
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

    IPanelWidget(const char* name, const ImVec2& initialSize, Position position, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

    Position GetPosition() const
    {
		return _position;
    }
    
private:
	void BeginWidget() override final;
	void EndWidget() override final;

	ImVec2 _initialSize;
	Position _position;
};
