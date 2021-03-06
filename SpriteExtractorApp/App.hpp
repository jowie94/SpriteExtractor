#pragma once

#include <string>
#include <mutex>

#include "Types.hpp"
#include "SpriteExtractor.h"
#include "CommandQueue/CommandQueue.hpp"

class SpriteSheet;

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
    virtual ~App();

    virtual void Run() = 0;

protected:
    void Init();
    void Loop();
    void Shutdown();

    virtual std::shared_ptr<IImage> OpenImage(const std::string& path) = 0;

private:
    // Callbacks
    void OnSelectFile();
    void OnSaveFile();
    void OnSearchSprites(const RightPanelActions::SearchSprites& action);
    void OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites);
    void OnCancelSearch(const MainWindowActions::CancelSearch& cancelSearch);

    std::shared_ptr<SpriteSheet> _currentSpriteSheet;

    SpriteExtractor::Task _searchSpritesTask;

    std::shared_ptr<class Scheduler> _scheduler;

    CommandQueue _commandQueue;
};
