#include "MainWindowWidget.hpp"

#include <string>

#include <imgui.h>

#include "RightPanelWidget.hpp"
#include "CentralPanelWidget.hpp"
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
    broker.Subscribe<GenericActions::SpriteSearchFinished>(std::bind(&MainWindowWidget::OnSpriteSearchFinished, this, std::placeholders::_1));
    broker.Subscribe<SpriteSearchMessages::ProgressUpdate>(std::bind(&MainWindowWidget::OnProgressUpdate, this, std::placeholders::_1));

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

    DrawSearchingPopup();

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
    _searchingPopupState = PopupState::Open;
}

void MainWindowWidget::OnSpriteSearchFinished(const GenericActions::SpriteSearchFinished& /*spriteSearchFinished*/)
{
    _searchingPopupState = PopupState::Close;
}

void MainWindowWidget::OnProgressUpdate(const SpriteSearchMessages::ProgressUpdate& progressUpdate)
{
    _stage = progressUpdate.Stage;
    _progress = progressUpdate.Progress;
}

void MainWindowWidget::DrawSearchingPopup()
{
    if (_searchingPopupState == PopupState::Open)
    {
        ImGui::OpenPopup("Searching Sprites");

        _searchingPopupState = PopupState::Opened;
    }

    if (ImGui::BeginPopupModal("Searching Sprites", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        std::string message;
        switch (_stage)
        {
        case SpriteExtractor::Task::Stage::GenerateMatrix:
        {
            message = "Generating binary matrix";
            break;
        }
        case SpriteExtractor::Task::Stage::FindSprites:
        {
            message = "Searching sprites";
            break;
        }
        default:
        {
            message = "ERROR: Unknown stage";
            break;
        }
        }

        ImGui::Text("%s", message.c_str());

        ImGui::ProgressBar(_progress);

        if (ImGui::Button("Cancel"))
        {
            MessageBroker::GetInstance().Broadcast(MainWindowActions::CancelSearch());
            _searchingPopupState = PopupState::Close;
        }

        if (_searchingPopupState == PopupState::Close)
        {
            ImGui::CloseCurrentPopup();
            _searchingPopupState = PopupState::Closed;
        }

        ImGui::EndPopup();
    }
}
