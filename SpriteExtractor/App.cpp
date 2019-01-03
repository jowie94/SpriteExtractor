#include "App.hpp"

#include "Platform/GenericPlatform.h"

#include <algorithm>

namespace AppConst
{
    static std::vector<Platform::FileFilter> kImgFilter =
    {
        { "Images", "*.png;*.jpg" },
        { "All", "*.*" }
    };

    float CalculateImageScale(const ITextureResource& image, const ImVec2& windowSize)
    {
        ImVec2 imgSize(static_cast<float>(image.size.x), static_cast<float>(image.size.y));

        if (windowSize.x > imgSize.x && windowSize.y > imgSize.y)
        {
            return 1.0f;
        }

        float scaleX = windowSize.x / imgSize.x;
        float scaleY = windowSize.y / imgSize.y;

        return std::min(scaleX, scaleY);
    }
}

namespace ImGui
{
    void Image(const ITextureResource& image, const ImVec2& imageSize)
    {
        ImGui::Image((void*)(intptr_t)image.resourceId, imageSize);
    }
}

void App::Loop()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        DrawFileMenu();
        ImGui::EndMenuBar();
    }

    ImGui::BeginChild("Image Container", ImVec2(-300.0f, -30.0f));
    DrawImageContainer();
    ImGui::EndChild();

    ImGui::SameLine(0.0f, 0.0f);

    ImGui::BeginChild("Right", ImVec2(300.0f, -30.0f));
    DrawRightPanel();
    ImGui::EndChild();

    ImGui::Separator();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 1.0f));
    ImGui::BeginChild("ContextPanel", ImVec2(0.0f, 20.0f));
    ImGui::Text("Opened image: %s", selectedFile.c_str());
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::End();
    ImGui::PopStyleVar(3);

    //ImGui::ShowTestWindow();
}

void App::DrawFileMenu()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open File"/*, "Ctrl + O"*/))
        {
            OnSelectFile();
        }

        ImGui::EndMenu();
    }
}

void App::DrawImageContainer()
{
    ImGui::BeginChild("Image", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    imageWindowSize = ImGui::GetWindowSize();
    if (openedImage)
    {
        ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
        ImGui::Image(*textureResource, ImVec2(textureResource->size.x * imageScale, textureResource->size.y * imageScale));

        for (const auto& sprite : foundSprites)
        {
            ImVec2 rectPos(cursorScreenPos.x + sprite.X * imageScale, cursorScreenPos.y + sprite.Y * imageScale);
            ImVec2 maxRect(rectPos.x + ((sprite.Width + 1.0f) * imageScale), rectPos.y + (sprite.Height + 1.0f) * imageScale);
            ImGui::GetWindowDrawList()->AddRect(rectPos, maxRect, ImColor(255, 0, 0));
        }

        /*ImVec2 rectPos(cursorScreenPos.x + 50.0f * imageScale, cursorScreenPos.y + 80.0f * imageScale);
        ImGui::GetWindowDrawList()->AddRect(rectPos, ImVec2((rectPos.x) + 120.0f * imageScale, (rectPos.y) + 120.0f * imageScale), ImColor(255, 0, 0));*/
    }
    ImGui::EndChild();

    ImVec2 cursorPos = ImGui::GetWindowSize();
    cursorPos.x -= 95.0f;
    cursorPos.y -= 50.0f;

    ImGui::SetCursorPos(cursorPos);

    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(0, 0, 0).Value);
    ImGui::BeginChild("Zoom", ImVec2(75.0f, 30.0f), true);
    if (ImGui::SmallButton("+"))
    {
        imageScale += 0.5f;
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("-"))
    {
        imageScale -= 0.1f;
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("="))
    {
        imageScale = AppConst::CalculateImageScale(*textureResource, imageWindowSize);
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void App::DrawRightPanel()
{
    /*ImGui::Text("File: %s", selectedFile.c_str());

    const bool button = ImGui::Button("Open File");

    if (button)
    {
        OnSelectFile();
    }*/

    if (ImGui::Button("Search Sprites"))
    {
        alphaColor = Color(128, 0, 255);
        OnSearchSprites();
    }
}

void App::OnSelectFile()
{
    if (Platform::ShowOpenFileDialogue("Choose an sprite sheet image", selectedFile, AppConst::kImgFilter))
    {
        openedImage = OpenImage(selectedFile);
        textureResource = openedImage->GetTextureResource();

        imageScale = AppConst::CalculateImageScale(*textureResource, imageWindowSize);

        foundSprites.clear();
    }
}

void App::OnSearchSprites()
{
    SpriteExtractor::ImageAccessor callbacks;
    callbacks.GetWidth = [](const void* image)
    {
        return static_cast<const IImage*>(image)->Size().x;
    };
    callbacks.GetHeight = [](const void* image)
    {
        return static_cast<const IImage*>(image)->Size().y;
    };
    callbacks.GetColor = [](size_t x, size_t y, const void* image)
    {
        return static_cast<const IImage*>(image)->GetPixel(x, y);
    };

    Matrix<bool> imageMatrix = SpriteExtractor::GenerateMatrix(callbacks, alphaColor, static_cast<const void*>(openedImage.get()));

    foundSprites = SpriteExtractor::FindSprites(imageMatrix);
}
