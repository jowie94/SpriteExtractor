#include "BaseMainWindow.hpp"

#include <cassert>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h> // Docking stuff

struct PanelContainer
{
    std::shared_ptr<PanelWindow> panel;
    bool positioned = false; // For future use
    PanelWindow::Position initialPosition;
};


BaseMainWindow::BaseMainWindow(const char* name)
: BaseWindow(name, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar)
{
}

BaseMainWindow::~BaseMainWindow() = default;

void BaseMainWindow::Draw()
{
    BaseWindow::Draw();

    ImGuiID dockId = ImGui::GetID(GetName());
    if (ImGui::DockBuilderGetNode(dockId) == nullptr)
    {
        SetupLayout();
    }

    ImGui::DockSpace(dockId);

	DrawPanels();

    _popupsController.Draw();
}

PopupsController& BaseMainWindow::GetPopupsController()
{
    return _popupsController;
}

void BaseMainWindow::AddPanelInt(PanelWindow::Position initialPosition, std::shared_ptr<PanelWindow> panel)
{
    panel->Init();

    std::lock_guard<std::mutex> lock(_panelsMutex);
    _panels.emplace_back(PanelContainer{ std::move(panel), false, initialPosition });
}

bool BaseMainWindow::BeginWidget()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    return ImGui::Begin(GetName(), nullptr, _flags);
}

void BaseMainWindow::EndWidget(bool wasDrawn)
{
    ImGui::End();
}

void BaseMainWindow::DrawPanels()
{
    std::lock_guard<std::mutex> lock(_panelsMutex);

	auto panelIt = _panels.begin();
	while (panelIt != _panels.end())
	{
		std::shared_ptr<PanelWindow> panelPtr = panelIt->panel;

		if (panelPtr->GetClosePolicy() == PanelWindow::ClosePolicy::NoClose || panelPtr->IsOpened())
		{
			if (!panelIt->positioned)
			{
				ImGui::SetNextWindowDockID(_dockIds[static_cast<size_t>(panelIt->initialPosition)], ImGuiCond_FirstUseEver);
				panelIt->positioned = true;
			}

			panelPtr->DoDraw();
		}

		if (panelPtr->GetClosePolicy() == PanelWindow::ClosePolicy::Close && !panelPtr->IsOpened())
		{
			panelIt = _panels.erase(panelIt);
		}
		else
		{
			++panelIt;
		}
	}
}

void BaseMainWindow::SetupLayout()
{
    ImGuiID dockId = ImGui::GetID(GetName());

    ImGui::DockBuilderRemoveNode(dockId);    // Clear out existing layout
    ImGui::DockBuilderAddNode(dockId);       // Add empty node
    ImGui::DockBuilderSetNodeSize(dockId, ImGui::GetIO().DisplaySize);

    ImGuiID mainDock = dockId;
    ImGuiID bottomDockId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Down, 0.20f, NULL, &mainDock);
    ImGuiID topDockId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Up, 0.20f, NULL, &mainDock);
    ImGuiID rightDockId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Right, 0.20f, NULL, &mainDock);
    ImGuiID leftDockId = ImGui::DockBuilderSplitNode(mainDock, ImGuiDir_Left, 0.20f, NULL, &mainDock);

    std::lock_guard<std::mutex> lock(_panelsMutex);
    for (auto& panel : _panels)
    {
        const char* panelName = panel.panel->GetName();
        switch (panel.initialPosition)
        {
            case PanelWindow::Position::Right:
            {
                ImGui::DockBuilderDockWindow(panelName, rightDockId);
                break;
            }
            case PanelWindow::Position::Left:
            {
                ImGui::DockBuilderDockWindow(panelName, leftDockId);
                break;
            }
            case PanelWindow::Position::Up:
            {
                ImGui::DockBuilderDockWindow(panelName, topDockId);
                break;
            }
            case PanelWindow::Position::Down:
            {
                ImGui::DockBuilderDockWindow(panelName, bottomDockId);
                break;
            }
            case PanelWindow::Position::Middle:
            {
                ImGui::DockBuilderDockWindow(panelName, mainDock);
                break;
            }
        }

        panel.positioned = true;
    }

	_dockIds[static_cast<size_t>(PanelWindow::Position::None)] = 0;
	_dockIds[static_cast<size_t>(PanelWindow::Position::Left)] = leftDockId;
	_dockIds[static_cast<size_t>(PanelWindow::Position::Middle)] = mainDock;
	_dockIds[static_cast<size_t>(PanelWindow::Position::Right)] = rightDockId;
	_dockIds[static_cast<size_t>(PanelWindow::Position::Up)] = topDockId;
	_dockIds[static_cast<size_t>(PanelWindow::Position::Down)] = bottomDockId;

    ImGui::DockBuilderFinish(dockId);
}
