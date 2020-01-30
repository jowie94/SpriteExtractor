#pragma once

#include <string>

#include "UI/BaseMainWindow.hpp"

namespace RightPanelActions 
{
    struct SearchSprites;
}

namespace GenericActions
{
    struct SpriteSearchFinished;
}

namespace SpriteSearchMessages
{
    struct ProgressUpdate;
}

class MainWindow : public BaseMainWindow
{
public:
	MainWindow();

    void Init() override;

protected:
    void BeforeDraw() override;
    void Draw() override;
    void AfterDraw() override;

private:
    // Panels
    void DrawMenuBar();
    void DrawFileMenu();
	void DrawAnimationMenu();
    void DrawDebugMenu();

    void OnSearchSprites(const RightPanelActions::SearchSprites& searchSprites);

    bool _showMetrics = false;

    std::weak_ptr<PanelWindow> _consolePanel;
    std::weak_ptr<PanelWindow> _infoPanel;
	std::weak_ptr<PopupWindow> _newAnimationPopup;
};
