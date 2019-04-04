#include "BaseMainWindow.hpp"

#include <cassert>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h> // Docking stuff

BaseMainWindow::BaseMainWindow(const char* name)
: BaseWindow(name, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar)
{
}

PopupsController& BaseMainWindow::GetPopupsController()
{
	return _popupsController;
}

void BaseMainWindow::AddPanelInt(std::shared_ptr<PanelWindow> panel)
{
	panel->Init();

	_panels.emplace_back(std::move(panel));
}

void BaseMainWindow::BeginWidget()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	ImGui::Begin(GetName(), nullptr, _flags);

	ImGuiID dockId = ImGui::GetID(GetName());
	if (ImGui::DockBuilderGetNode(dockId) == nullptr)
	{
		SetupLayout();
	}

	ImGui::DockSpace(dockId);

	for (auto& panel : _panels)
	{
		panel->DoDraw();
	}

	_popupsController.Draw();
}

void BaseMainWindow::EndWidget()
{
	ImGui::End();
}

void BaseMainWindow::SetupLayout()
{
	ImGuiID dockId = ImGui::GetID(GetName());

	ImGui::DockBuilderRemoveNode(dockId);                                  // Clear out existing layout
	ImGui::DockBuilderAddNode(dockId);       // Add empty node
	ImGui::DockBuilderSetNodeSize(dockId, ImGui::GetIO().DisplaySize);

	ImGuiID mainDock = dockId;
	ImGuiID rightDockId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Right, 0.20f, NULL, &mainDock);
	ImGuiID leftDockId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Left, 0.20f, NULL, &mainDock);
	ImGuiID topDockId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Up, 0.20f, NULL, &mainDock);
	ImGuiID bottomDockId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Down, 0.20f, NULL, &mainDock);

	for (const auto& panel : _panels)
	{
		switch (panel->GetPosition())
		{
			case PanelWindow::Position::Right:
			{
				ImGui::DockBuilderDockWindow(panel->GetName(), rightDockId);
				break;
			}
			case PanelWindow::Position::Left:
			{
				ImGui::DockBuilderDockWindow(panel->GetName(), leftDockId);
				break;
			}
			case PanelWindow::Position::Up:
			{
				ImGui::DockBuilderDockWindow(panel->GetName(), topDockId);
				break;
			}
			case PanelWindow::Position::Down:
			{
				ImGui::DockBuilderDockWindow(panel->GetName(), bottomDockId);
				break;
			}
			case PanelWindow::Position::Middle:
			{
				ImGui::DockBuilderDockWindow(panel->GetName(), mainDock);
			}
		}
	}

	ImGui::DockBuilderFinish(dockId);
}
