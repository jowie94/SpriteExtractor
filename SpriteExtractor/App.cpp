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

    bool Button(const char* label, bool enabled, const ImVec2& size = ImVec2(0, 0))
    {
        if (!enabled)
        {
            PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Button));
            PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_Button));
        }

        bool result = Button(label, size);

        if (!enabled)
        {
            PopStyleVar();
            PopStyleColor(2);
        }

        return enabled && result;
    }
}

App::App()
: searchSpritesTask(std::bind(&App::OnSpritesFound, this, std::placeholders::_1))
{}

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

    DrawSearchingPopup();

    ImGui::PopStyleVar(3);

    //ImGui::ShowTestWindow();
}

void App::DrawFileMenu()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open File", nullptr, false))
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

        if (enableColorPicker)
        {
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 relativeMousePos((mousePos.x - cursorScreenPos.x) / imageScale, (mousePos.y - cursorScreenPos.y) / imageScale);

            if (ImGui::IsWindowHovered() && relativeMousePos.x >= 0 && relativeMousePos.x <= openedImage->Size().x && relativeMousePos.y >= 0 && relativeMousePos.y <= openedImage->Size().y)
            {
                alphaColor = openedImage->GetPixel(static_cast<unsigned int>(relativeMousePos.x), static_cast<unsigned int>(relativeMousePos.y));
            }
            else
            {
                alphaColor = originalAlphaColor;
            }

            if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
            {
                enableColorPicker = false;
            }
        }


        std::lock_guard<std::mutex> spritesLock(foundSpritesMutex);
        for (const auto& sprite : foundSprites)
        {
            ImVec2 rectPos(cursorScreenPos.x + sprite.X * imageScale, cursorScreenPos.y + sprite.Y * imageScale);
            ImVec2 maxRect(rectPos.x + ((sprite.Width + 1.0f) * imageScale), rectPos.y + (sprite.Height + 1.0f) * imageScale);
            ImGui::GetWindowDrawList()->AddRect(rectPos, maxRect, ImColor(255, 0, 0));
        }
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
    float col4[4];

    alphaColor.ToFloat(col4);
    if (ImGui::ColorPicker4("Alpha Color", col4))
    {
        alphaColor = col4;
    }

    bool colorPickerEnabled = enableColorPicker;

    if (colorPickerEnabled)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImColor(96, 198, 53).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(74, 153, 41).Value);
    }
    if (ImGui::Button("Pick Color"))
    {
        if (enableColorPicker)
        {
            alphaColor = originalAlphaColor;
            enableColorPicker = false;
        }
        else
        {
            originalAlphaColor = alphaColor;
            enableColorPicker = true;
        }
    }
    if (colorPickerEnabled)
    {
        ImGui::PopStyleColor(2);
    }

    if (ImGui::Button("Search Sprites", openedImage != nullptr))
    {
        OnSearchSprites();
    }
}

void App::DrawSearchingPopup()
{
    if (searchingPopupState == PopupState::Open)
    {
        ImGui::OpenPopup("Searching Sprites");

        searchingPopupState = PopupState::Opened;
    }

    if (ImGui::BeginPopupModal("Searching Sprites", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        std::string message;
        switch (searchSpritesTask.GetStage())
        {
            case SpriteExtractor::Task::Stage::GenerateMatrix:
            {
                message = "Generating binary matrix";
                break;
            }
            case SpriteExtractor::Task::Stage::FindSprites:
            {
                message = "Searching sprites";
                break;
            }
            default:
            {
                message = "ERROR: Unknown stage";
                break;
            }
        }

        ImGui::Text("%s", message.c_str());

        ImGui::ProgressBar(searchSpritesTask.GetProgress());

        if (ImGui::Button("Cancel"))
        {
            OnCancelSearch();
        }

        if (searchingPopupState == PopupState::Close)
        {
            ImGui::CloseCurrentPopup();
            searchingPopupState = PopupState::Closed;
        }

        ImGui::EndPopup();
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
    {
        std::lock_guard<std::mutex> spriteList(foundSpritesMutex);
        foundSprites.clear();
    }

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

    searchingPopupState = PopupState::Open;
    searchSpritesTask.Run(callbacks, alphaColor, static_cast<const void*>(openedImage.get()));
}

void App::OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites)
{
    std::lock_guard<std::mutex> spriteList(foundSpritesMutex);
    this->foundSprites = foundSprites;

    searchingPopupState = PopupState::Close;
}

void App::OnCancelSearch()
{
    searchSpritesTask.Stop();

    searchingPopupState = PopupState::Close;
}
