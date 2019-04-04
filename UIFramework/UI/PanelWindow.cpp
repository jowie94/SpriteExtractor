#include "PanelWindow.hpp"

PanelWindow::PanelWindow(const char* name, const ImVec2& initialSize, Position position, ImGuiWindowFlags flags)
: BaseWindow(name, flags)
, _initialSize(initialSize)
, _position(position)
{
}

void PanelWindow::BeginWidget()
{
	ImGui::SetNextWindowSize(_initialSize, ImGuiCond_FirstUseEver);
	ImGui::Begin(GetName(), nullptr, _flags);
}

void PanelWindow::EndWidget()
{
	ImGui::End();
}
