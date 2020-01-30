#include "MainWindow.hpp"

#include <ImGui/imgui.h>

#include "Windows/Panels/SearchConfigPanel.hpp"
#include "Windows/Panels/SpritesPanel.hpp"
#include "Windows/Panels/ConsolePanel.hpp"
#include "Windows/Panels/SpriteInfoPanel.hpp"

#include "Windows/Popups/SearchingPopup.hpp"
#include "Windows/Popups/NewAnimationPopup.hpp"

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

    _consolePanel = AddPanel<ConsolePanel>(PanelWindow::Position::Down);

    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe<RightPanelActions::SearchSprites>(std::bind(&MainWindow::OnSearchSprites, this, std::placeholders::_1));

    auto imageOpenedCallback = [this](const SpriteSearchMessages::SpriteSearchFinished&)
    {
        if (auto panel = _infoPanel.lock())
        {
            panel->RequestFocus();
        }
        else
        {
            _infoPanel = AddPanel<SpriteInfoPanel>(PanelWindow::Position::Right);
        }
    };
    broker.Subscribe<SpriteSearchMessages::SpriteSearchFinished>(imageOpenedCallback);
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
		DrawAnimationMenu();
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

void MainWindow::DrawAnimationMenu()
{
	if (ImGui::BeginMenu("Animation"))
	{
		if (ImGui::MenuItem("New Animation", nullptr, false))
		{
			auto popup = std::make_shared<NewAnimationPopup>();
			popup->Init();
			GetPopupsController().ShowPopup(popup);

			_newAnimationPopup = popup;
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

        auto consolePtr = _consolePanel.lock();
        bool consoleOpened = consolePtr && consolePtr->IsOpened();
        if (ImGui::MenuItem("Debug Console", nullptr, consoleOpened))
        {
            if (!consoleOpened)
            {
                _consolePanel = AddPanel<ConsolePanel>(PanelWindow::Position::None);
            }
            else if (consolePtr)
            {
                consolePtr->Close();
            }
            
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
