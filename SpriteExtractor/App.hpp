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

namespace MainWindowActions
{
    struct CancelSearch;
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
    // Callbacks
    void OnSelectFile();
    void OnSaveFile();
    void OnSearchSprites(const RightPanelActions::SearchSprites& action);
    void OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites);
    void OnCancelSearch(const MainWindowActions::CancelSearch& cancelSearch);

    std::string _selectedFile;
    std::shared_ptr<IImage> _openedImage;

    std::mutex _foundSpritesMutex;
    std::shared_ptr<SpriteList> _foundSprites;

    SpriteExtractor::Task _searchSpritesTask;

    std::unique_ptr<IWidget> _mainWindow;
};
