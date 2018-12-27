#pragma once

#include <string>
#include <imgui.h>

#include "Types.hpp"

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

    std::string selectedFile;
    std::unique_ptr<IImage> openedImage;
    std::unique_ptr<ITextureResource> textureResource;

    ImVec2 imageWindowSize;
    float imageScale;
};
