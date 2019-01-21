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
    void OnSaveFile();
    void OnSearchSprites();
    void OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites);
    void OnCancelSearch();

    std::string _selectedFile;
    std::unique_ptr<IImage> _openedImage;
    std::unique_ptr<ITextureResource> _textureResource;

    Color _alphaColor;
    ImVec2 _imageWindowSize;
    float _imageScale = 1.0f;

    std::mutex _foundSpritesMutex;
    SpriteExtractor::SpriteList _foundSprites;

    SpriteExtractor::Task _searchSpritesTask;

    PopupState _searchingPopupState = PopupState::Closed;

    Color _originalAlphaColor;
    bool _enableColorPicker = false;

    bool _showMetrics = false;
};
