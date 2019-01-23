#include "App.hpp"

#include "Platform/GenericPlatform.h"
#include "Serializers/Serializer.hpp"
#include "MessageBroker.hpp"
#include "Messages/GenericActions.hpp"

#include "Widgets/RightPanelWidget.hpp"

#include "imgui-extra.hpp"

#include <algorithm>
#include "Messages/RightPanelActions.hpp"

namespace AppConst
{
    float CalculateImageScale(const ITextureResource& image, const ImVec2& windowSize)
    {
        ImVec2 imgSize(static_cast<float>(image.Size.X), static_cast<float>(image.Size.Y));

        if (windowSize.x > imgSize.x && windowSize.y > imgSize.y)
        {
            return 1.0f;
        }

        float scaleX = windowSize.x / imgSize.x;
        float scaleY = windowSize.y / imgSize.y;

        return std::min(scaleX, scaleY);
    }

    void ReplaceExtension(std::string& path, const std::string& newExt)
    {
        size_t dotPos = path.rfind('.', path.length());

        if (dotPos != std::string::npos)
        {
            path.replace(dotPos+1, path.length(), newExt);
        }
    }

    static std::vector<Platform::FileFilter> kImgFilter =
    {
        { "Images", "*.png;*.jpg" },
        { "All", "*.*" }
    };

    float kZoomFactor = 0.3f;
}

App::App()
: _searchSpritesTask(std::bind(&App::OnSpritesFound, this, std::placeholders::_1))
{}

void App::Init()
{
    _rightWidget = std::make_unique<RightPanelWidget>();
    _rightWidget->Init();

    MessageBroker& broker = MessageBroker::GetInstance();

    auto searchSpritesCB = [this](const RightPanelActions::SearchSprites&)
    {
        OnSearchSprites();
    };
    broker.Subscribe(MessageCallback<RightPanelActions::SearchSprites>(searchSpritesCB));

    auto saveSpritesCB = [this](const RightPanelActions::SaveFile&)
    {
        OnSaveFile();
    };
    broker.Subscribe(MessageCallback<RightPanelActions::SaveFile>(saveSpritesCB));

    auto toggleColorPickerCB = [this](const RightPanelActions::ToggleColorPicker& toggle)
    {
        _enableColorPicker = toggle.Enabled;

        if (_enableColorPicker)
        {
            _originalAlphaColor = _alphaColor;
        }
    };
    broker.Subscribe(MessageCallback<RightPanelActions::ToggleColorPicker>(toggleColorPickerCB));
}

void App::Loop()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

    DrawMenuBar();

    ImGui::BeginChild("Image Container", ImVec2(-300.0f, -30.0f));
    DrawImageContainer();
    ImGui::EndChild();

    ImGui::SameLine(0.0f, 0.0f);

    //DrawRightPanel();
    _rightWidget->Draw();

    ImGui::Separator();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 1.0f));
    ImGui::BeginChild("ContextPanel", ImVec2(0.0f, 20.0f));
    ImGui::Text("Opened image: %s", _selectedFile.c_str());
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::End();

    DrawSearchingPopup();

    if (_showMetrics)
    {
        ImGui::ShowMetricsWindow(&_showMetrics);
    }

    ImGui::PopStyleVar(3);

    //ImGui::ShowTestWindow();
}

void App::DrawMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        DrawFileMenu();
        DrawDebugMenu();
        ImGui::EndMenuBar();
    }
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

void App::DrawDebugMenu()
{
    if (ImGui::BeginMenu("Debug"))
    {
        if (ImGui::MenuItem("Show Metrics", nullptr, _showMetrics))
        {
            _showMetrics = !_showMetrics;
        }

        ImGui::EndMenu();
    }
}

void App::DrawImageContainer()
{
    ImGui::BeginChild("Image", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    _imageWindowSize = ImGui::GetWindowSize();
    if (_openedImage)
    {
        ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
        ImGui::Image(*_textureResource, ImVec2(_textureResource->Size.X * _imageScale, _textureResource->Size.Y * _imageScale));

        if (_enableColorPicker)
        {
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 relativeMousePos((mousePos.x - cursorScreenPos.x) / _imageScale, (mousePos.y - cursorScreenPos.y) / _imageScale);

            MessageBroker& broker = MessageBroker::GetInstance();

            if (ImGui::IsWindowHovered() && relativeMousePos.x >= 0 && relativeMousePos.x <= _openedImage->Size().X && relativeMousePos.y >= 0 && relativeMousePos.y <= _openedImage->Size().Y)
            {
                _alphaColor = _openedImage->GetPixel(static_cast<unsigned int>(relativeMousePos.x), static_cast<unsigned int>(relativeMousePos.y));
            }
            else
            {
                _alphaColor = _originalAlphaColor;
            }

            broker.Broadcast(GenericActions::ColorHovered{_alphaColor});

            if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
            {
                _enableColorPicker = false;
                broker.Broadcast(GenericActions::ColorPicked{_alphaColor});
            }
        }


        std::lock_guard<std::mutex> spritesLock(_foundSpritesMutex);
        for (const auto& sprite : _foundSprites)
        {
            ImVec2 rectPos(cursorScreenPos.x + sprite.X * _imageScale, cursorScreenPos.y + sprite.Y * _imageScale);
            ImVec2 maxRect(rectPos.x + ((sprite.Width + 1.0f) * _imageScale), rectPos.y + (sprite.Height + 1.0f) * _imageScale);
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
    if (ImGui::SmallButton("+") && _imageScale > AppConst::kZoomFactor)
    {
        _imageScale += AppConst::kZoomFactor;
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("-") && _imageScale > AppConst::kZoomFactor)
    {
        _imageScale -= AppConst::kZoomFactor;
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("="))
    {
        _imageScale = AppConst::CalculateImageScale(*_textureResource, _imageWindowSize);
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void App::DrawRightPanel()
{
    float col4[4];

    _alphaColor.ToFloat(col4);
    if (ImGui::ColorPicker4("Alpha Color", col4))
    {
        _alphaColor = col4;
    }

    bool colorPickerEnabled = _enableColorPicker;

    if (colorPickerEnabled)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImColor(96, 198, 53).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(74, 153, 41).Value);
    }
    if (ImGui::Button("Pick Color"))
    {
        if (_enableColorPicker)
        {
            _alphaColor = _originalAlphaColor;
            _enableColorPicker = false;
        }
        else
        {
            _originalAlphaColor = _alphaColor;
            _enableColorPicker = true;
        }
    }
    if (colorPickerEnabled)
    {
        ImGui::PopStyleColor(2);
    }

    if (ImGui::Button("Search Sprites", _openedImage != nullptr))
    {
        OnSearchSprites();
    }

    if (ImGui::Button("Save", !_foundSprites.empty()))
    {
        OnSaveFile();
    }
}

void App::DrawSearchingPopup()
{
    if (_searchingPopupState == PopupState::Open)
    {
        ImGui::OpenPopup("Searching Sprites");

        _searchingPopupState = PopupState::Opened;
    }

    if (ImGui::BeginPopupModal("Searching Sprites", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        std::string message;
        switch (_searchSpritesTask.GetStage())
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

        ImGui::ProgressBar(_searchSpritesTask.GetProgress());

        if (ImGui::Button("Cancel"))
        {
            OnCancelSearch();
        }

        if (_searchingPopupState == PopupState::Close)
        {
            ImGui::CloseCurrentPopup();
            _searchingPopupState = PopupState::Closed;
        }

        ImGui::EndPopup();
    }
}

void App::OnSelectFile()
{
    if (Platform::ShowOpenFileDialogue("Choose an sprite sheet image", _selectedFile, AppConst::kImgFilter))
    {
        MessageBroker& broker = MessageBroker::GetInstance();

        _openedImage = OpenImage(_selectedFile);
        broker.Broadcast(GenericActions::ImageOpened(_openedImage));
        
        _textureResource = _openedImage->GetTextureResource();

        _imageScale = AppConst::CalculateImageScale(*_textureResource, _imageWindowSize);

        _foundSprites.clear();
    }
}

void App::OnSaveFile()
{
    std::string outFile;
    if (Platform::ShowSaveFileDialogue("Save Sprites", outFile, Serializer::GetSerializerFilters()))
    {
        Serializer::Serialize(outFile, _foundSprites);

        AppConst::ReplaceExtension(outFile, "png");
        _openedImage->Save(outFile.c_str());
    }
}

void App::OnSearchSprites()
{
    {
        std::lock_guard<std::mutex> spriteList(_foundSpritesMutex);
        _foundSprites.clear();
    }

    SpriteExtractor::ImageAccessor callbacks;
    callbacks.GetWidth = [](const void* image)
    {
        return static_cast<const IImage*>(image)->Size().X;
    };
    callbacks.GetHeight = [](const void* image)
    {
        return static_cast<const IImage*>(image)->Size().Y;
    };
    callbacks.GetColor = [](size_t x, size_t y, const void* image)
    {
        return static_cast<const IImage*>(image)->GetPixel(x, y);
    };

    _searchingPopupState = PopupState::Open;
    _searchSpritesTask.Run(callbacks, _alphaColor, static_cast<const void*>(_openedImage.get()));
}

void App::OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites)
{
    std::lock_guard<std::mutex> spriteList(_foundSpritesMutex);
    _foundSprites = foundSprites;

    MessageBroker::GetInstance().Broadcast(GenericActions::SpriteSearchFinished(_foundSprites));

    _searchingPopupState = PopupState::Close;
}

void App::OnCancelSearch()
{
    _searchSpritesTask.Stop();

    _searchingPopupState = PopupState::Close;
}
