#include "App.hpp"

#include "Platform/GenericPlatform.h"

#include <imgui.h>

namespace AppConst
{
    static std::vector<Platform::FileFilter> kImgFilter =
    {
        { "Images", "*.png;*.jpg" },
        { "All", "*.*" }
    };
}

namespace ImGui
{
    void Image(const ITextureResource& image)
    {
        ImVec2 imgSize(image.size.x, image.size.y);
        ImGui::Image((void*)(intptr_t)image.resourceId, imgSize);
    }
}

void App::Loop()
{
    //ImGui::ShowTestWindow();

    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::Text("File: %s", selectedFile.c_str());

    const bool button = ImGui::Button("Open File");

    if (button)
    {
        std::string file;
        if (Platform::ShowOpenFileDialogue("Choose an sprite sheet image", file, AppConst::kImgFilter))
        {
            selectedFile = file;
            openedImage = OpenImage(selectedFile);
            textureResource = openedImage->GetTextureResource();
        }
    }

    if (openedImage)
    {
        ImGui::Image(*textureResource);
    }

    ImGui::End();
}
