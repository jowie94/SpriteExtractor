#include "PanelWindow.hpp"

PanelWindow::PanelWindow(const char* name, const ImVec2& initialSize, ImGuiWindowFlags flags)
: BaseWindow(name, flags)
, _initialSize(initialSize)
{
}

PanelWindow::ClosePolicy PanelWindow::GetClosePolicy() const
{
	return _closePolicy;
}

bool PanelWindow::IsOpened() const
{
	return _opened;
}

void PanelWindow::SetClosePolicy(ClosePolicy policy)
{
	_closePolicy = policy;
}

bool PanelWindow::BeginWidget()
{
	bool* opened = _closePolicy != ClosePolicy::NoClose ? &_opened : nullptr;
    ImGui::SetNextWindowSize(_initialSize, ImGuiCond_FirstUseEver);
    return ImGui::Begin(GetName(), opened, _flags);
}

void PanelWindow::EndWidget(bool wasDrawn)
{
    ImGui::End();
}
