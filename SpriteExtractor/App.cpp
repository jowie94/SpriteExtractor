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

void App::Loop()
{
    //ImGui::ShowTestWindow();

    ImGui::Begin("");

    ImGui::Text("File: %s", selectedFile.c_str());

    const bool button = ImGui::Button("Open File");

    if (button)
    {
        std::string file;
        if (Platform::ShowOpenFileDialogue("Choose an sprite sheet image", file, AppConst::kImgFilter))
        {
            selectedFile = file;
        }
    }

    ImGui::End();
}
