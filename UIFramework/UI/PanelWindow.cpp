#include "PanelWindow.hpp"

PanelWindow::PanelWindow(const char* name, const ImVec2& initialSize, Position position, ImGuiWindowFlags flags)
: BaseWindow(name, flags)
, _initialSize(initialSize)
, _position(position)
{
}

bool PanelWindow::BeginWidget()
{
	ImGui::SetNextWindowSize(_initialSize, ImGuiCond_FirstUseEver);
	return ImGui::Begin(GetName(), nullptr, _flags);
}

void PanelWindow::EndWidget(bool wasDrawn)
{
	ImGui::End();
}
