#include "MainWindowWidget.hpp"

#include <ImGui/imgui.h>

#include "RightPanelWidget.hpp"
#include "CentralPanelWidget.hpp"

#include "UI/Popups/SearchingPopup.hpp"

#include "SpriteExtractor.h"

#include "MessageBroker.hpp"
#include "Messages/GenericActions.hpp"
#include "Messages/MainWindowActions.hpp"
#include "Messages/RightPanelActions.hpp"
#include "Messages/SpriteSearchMessages.hpp"

MainWindowWidget::MainWindowWidget()
: IMainWindowWidget("MainWindow")
{
}

void MainWindowWidget::Init()
{
    IMainWindowWidget::Init();

	AddPanel<CentralPanelWidget>(IPanelWidget::Position::Left);
	AddPanel<RightPanelWidget>(IPanelWidget::Position::Right);

    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe<RightPanelActions::SearchSprites>(std::bind(&MainWindowWidget::OnSearchSprites, this, std::placeholders::_1));

    auto imageOpenedCallback = [this](const GenericActions::ImageOpened& imageOpened)
    {
        _openedFile = imageOpened.ImageName;
    };
    broker.Subscribe<GenericActions::ImageOpened>(imageOpenedCallback);
}

void MainWindowWidget::BeforeDraw()
{
    IMainWindowWidget::BeforeDraw();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
}

void MainWindowWidget::Draw()
{
	IMainWindowWidget::Draw();

    DrawMenuBar();

    if (_showMetrics)
    {
        ImGui::ShowMetricsWindow(&_showMetrics);
    }
}

void MainWindowWidget::AfterDraw()
{
    IMainWindowWidget::AfterDraw();
    ImGui::PopStyleVar(3);
}

void MainWindowWidget::DrawMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        DrawFileMenu();
        DrawDebugMenu();
        ImGui::EndMenuBar();
    }
}

void MainWindowWidget::DrawFileMenu()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open File", nullptr, false))
        {
            MessageBroker::GetInstance().Broadcast(MainWindowActions::SelectFile());
        }

        ImGui::EndMenu();
    }
}

void MainWindowWidget::DrawDebugMenu()
{
    if (ImGui::BeginMenu("Debug"))
    {        
        if (ImGui::MenuItem("Show Metrics", nullptr, _showMetrics))
        {
            _showMetrics = !_showMetrics;
        }

        ImGui::EndMenu();
    }
}

void MainWindowWidget::OnSearchSprites(const RightPanelActions::SearchSprites& /*searchSprites*/)
{
    std::shared_ptr<SearchingPopup> searchingPopup = std::make_shared<SearchingPopup>();
    searchingPopup->Init();
    GetPopupsController().ShowPopup(searchingPopup);
}
