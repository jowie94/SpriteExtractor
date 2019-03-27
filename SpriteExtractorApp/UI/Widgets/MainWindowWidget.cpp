#include "MainWindowWidget.hpp"

#include <string>

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

void MainWindowWidget::Init()
{
    _rightWidget = std::make_unique<RightPanelWidget>();
    _rightWidget->Init();

    _centerWidget = std::make_unique<CentralPanelWidget>();
    _centerWidget->Init();

    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe<RightPanelActions::SearchSprites>(std::bind(&MainWindowWidget::OnSearchSprites, this, std::placeholders::_1));

    auto imageOpenedCallback = [this](const GenericActions::ImageOpened& imageOpened)
    {
        _openedFile = imageOpened.ImageName;
    };
    broker.Subscribe<GenericActions::ImageOpened>(imageOpenedCallback);
}

void MainWindowWidget::Draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
    _popupsController.Draw();

    DrawMenuBar();

    _centerWidget->Draw();
    ImGui::SameLine(0.0f, 0.0f);
    _rightWidget->Draw();

    ImGui::Separator();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 1.0f));
    ImGui::BeginChild("ContextPanel", ImVec2(0.0f, 20.0f));
    ImGui::Text("Opened image: %s", _openedFile.c_str());
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::End();
    // DrawSearchingPopup();

    if (_showMetrics)
    {
        ImGui::ShowMetricsWindow(&_showMetrics);
    }

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
    _popupsController.ShowPopup(searchingPopup);
    // _searchingPopupState = PopupState::Open;
}
