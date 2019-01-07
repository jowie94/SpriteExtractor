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
    void DrawFileMenu();
    void DrawImageContainer();
    void DrawRightPanel();

    void OnSelectFile();
    void OnSearchSprites();

    std::string selectedFile;
    std::unique_ptr<IImage> openedImage;
    std::unique_ptr<ITextureResource> textureResource;

    Color alphaColor;
    ImVec2 imageWindowSize;
    float imageScale;

    std::mutex foundSpritesMutex;
    SpriteExtractor::SpriteList foundSprites;

    std::atomic_bool isSearchingSprites;
    std::atomic_bool spritesProcessed;
    std::thread searchSpritesThread;
};
