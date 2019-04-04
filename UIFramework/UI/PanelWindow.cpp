#include "PanelWindow.hpp"

PanelWindow::PanelWindow(const char* name, const ImVec2& initialSize, ImGuiWindowFlags flags)
: BaseWindow(name, flags)
, _initialSize(initialSize)
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
