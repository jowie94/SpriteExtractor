#pragma once

#include <string>
#include <imgui.h>

#include "Types.hpp"
#include "SpriteExtractor.h"
#include <mutex>
#include <atomic>
#include <thread>

class App
{
public:
    App();
    virtual ~App() = default;

    virtual void Run() = 0;

protected:
    void Loop();

    virtual std::unique_ptr<IImage> OpenImage(const std::string& path) = 0;

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
    void DrawImageContainer();
    void DrawRightPanel();

    // Popups
    void DrawSearchingPopup();

    // Callbacks
    void OnSelectFile();
    void OnSearchSprites();
    void OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites);
    void OnCancelSearch();

    std::string selectedFile;
    std::unique_ptr<IImage> openedImage;
    std::unique_ptr<ITextureResource> textureResource;

    Color alphaColor;
    ImVec2 imageWindowSize;
    float imageScale = 1.0f;

    std::mutex foundSpritesMutex;
    SpriteExtractor::SpriteList foundSprites;

    SpriteExtractor::Task searchSpritesTask;

    PopupState searchingPopupState = PopupState::Closed;

    Color originalAlphaColor;
    bool enableColorPicker = false;

    bool showMetrics = false;
};
