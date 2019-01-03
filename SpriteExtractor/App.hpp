#pragma once

#include <string>
#include <imgui.h>

#include "Types.hpp"
#include "SpriteExtractor.h"

class App
{
public:
    App() = default;
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
    SpriteExtractor::SpriteList foundSprites;
};
