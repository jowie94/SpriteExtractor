#pragma once

#include <memory>
#include <string>

#include "IWidget.hpp"
#include "UI/PopupsController.hpp"

#include "SpriteExtractor.h"

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

class MainWindowWidget : public IWidget
{
public:
    void Init() override;
    void Draw() override;

private:
    enum class PopupState
    {
        Open,
        Opened,
        Close,
        Closed
    };

    // Panels
    void DrawMenuBar();
    void DrawFileMenu();
    void DrawDebugMenu();

    void OnSearchSprites(const RightPanelActions::SearchSprites& searchSprites);

    bool _showMetrics = false;

    std::string _openedFile;

    std::unique_ptr<IWidget> _rightWidget;
    std::unique_ptr<IWidget> _centerWidget;

    PopupsController _popupsController;
};
