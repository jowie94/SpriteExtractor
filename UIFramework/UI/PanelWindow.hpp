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
        Down,
		_Size
    };

	enum class ClosePolicy
	{
		NoClose,
		Close,
		Hide
	};

    PanelWindow(const char* name, const ImVec2& initialSize, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

	ClosePolicy GetClosePolicy() const;
    bool IsOpened() const;
    void Close();

protected:
	void SetClosePolicy(ClosePolicy policy);

	bool _opened = true;

private:
    bool BeginWidget() override final;
    void EndWidget(bool wasDrawn) override final;

    ImVec2 _initialSize;
	ClosePolicy _closePolicy = ClosePolicy::NoClose;
};
