#pragma once

#include <memory>
#include <string>

#include "IWidget.hpp"

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
    void OnSpriteSearchFinished(const GenericActions::SpriteSearchFinished& spriteSearchFinished);
    void OnProgressUpdate(const SpriteSearchMessages::ProgressUpdate& progressUpdate);

    void DrawSearchingPopup();

    bool _showMetrics = false;

    std::string _openedFile;

    // Search sprites popup
    PopupState _searchingPopupState = PopupState::Closed;
    float _progress = 0.0f;
    SpriteExtractor::Task::Stage _stage = SpriteExtractor::Task::Stage::None;

    std::unique_ptr<IWidget> _rightWidget;
    std::unique_ptr<IWidget> _centerWidget;
};
