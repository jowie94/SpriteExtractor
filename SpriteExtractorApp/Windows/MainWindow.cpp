#include "MainWindow.hpp"

#include <ImGui/imgui.h>

#include "Windows/Panels/SearchConfigPanel.hpp"
#include "Windows/Panels/SpritesPanel.hpp"

#include "Windows/Popups/SearchingPopup.hpp"

#include "SpriteExtractor.h"

#include "MessageBroker.hpp"
#include "Messages/GenericActions.hpp"
#include "Messages/MainWindowActions.hpp"
#include "Messages/RightPanelActions.hpp"
#include "Messages/SpriteSearchMessages.hpp"

MainWindow::MainWindow()
: BaseMainWindow("MainWindow")
{
}

void MainWindow::Init()
{
    BaseMainWindow::Init();

    AddPanel<SpritesPanel>(PanelWindow::Position::Left);
    AddPanel<SearchConfigPanel>(PanelWindow::Position::Right);

    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe<RightPanelActions::SearchSprites>(std::bind(&MainWindow::OnSearchSprites, this, std::placeholders::_1));

    auto imageOpenedCallback = [this](const GenericActions::ImageOpened& imageOpened)
    {
        _openedFile = imageOpened.ImageName;
    };
    broker.Subscribe<GenericActions::ImageOpened>(imageOpenedCallback);
}

void MainWindow::BeforeDraw()
{
    BaseMainWindow::BeforeDraw();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
}

void MainWindow::Draw()
{
    BaseMainWindow::Draw();

    DrawMenuBar();

    if (_showMetrics)
    {
        ImGui::ShowMetricsWindow(&_showMetrics);
    }
}

void MainWindow::AfterDraw()
{
    BaseMainWindow::AfterDraw();
    ImGui::PopStyleVar(3);
}

void MainWindow::DrawMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        DrawFileMenu();
        DrawDebugMenu();
        ImGui::EndMenuBar();
    }
}

void MainWindow::DrawFileMenu()
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

void MainWindow::DrawDebugMenu()
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

void MainWindow::OnSearchSprites(const RightPanelActions::SearchSprites& /*searchSprites*/)
{
    std::shared_ptr<SearchingPopup> searchingPopup = std::make_shared<SearchingPopup>();
    searchingPopup->Init();
    GetPopupsController().ShowPopup(searchingPopup);
}
