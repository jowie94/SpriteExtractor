#pragma once

#include <string>
#include <mutex>

#include "Types.hpp"
#include "SpriteExtractor.h"
#include "Widgets/IWidget.hpp"

namespace RightPanelActions 
{
    struct SearchSprites;
}

class App
{
public:
    App();
    virtual ~App() = default;

    void Init();
    virtual void Run() = 0;

protected:
    void Loop();

    virtual std::shared_ptr<IImage> OpenImage(const std::string& path) = 0;

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

    // Popups
    void DrawSearchingPopup();

    // Callbacks
    void OnSelectFile();
    void OnSaveFile();
    void OnSearchSprites(const RightPanelActions::SearchSprites& action);
    void OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites);
    void OnCancelSearch();

    std::string _selectedFile;
    std::shared_ptr<IImage> _openedImage;

    std::mutex _foundSpritesMutex;
    SpriteExtractor::SpriteList _foundSprites;

    SpriteExtractor::Task _searchSpritesTask;

    PopupState _searchingPopupState = PopupState::Closed;

    bool _showMetrics = false;

    std::unique_ptr<IWidget> _rightWidget;
    std::unique_ptr<IWidget> _centerWidget;
};
