#include "IPanelWidget.hpp"

IPanelWidget::IPanelWidget(const char* name, const ImVec2& initialSize, Position position, ImGuiWindowFlags flags)
: IWidget(name, flags)
, _initialSize(initialSize)
, _position(position)
{
}

void IPanelWidget::BeginWidget()
{
	ImGui::SetNextWindowSize(_initialSize, ImGuiCond_FirstUseEver);
	ImGui::Begin(GetName(), nullptr, _flags);
}

void IPanelWidget::EndWidget()
{
	ImGui::End();
}
