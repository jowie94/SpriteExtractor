#include "App.hpp"

#include "Platform/GenericPlatform.h"
#include "Serializers/Serializer.hpp"

#include "MessageBroker.hpp"
#include "Messages/GenericActions.hpp"
#include "Messages/RightPanelActions.hpp"

#include "Widgets/RightPanelWidget.hpp"

#include "imgui-extra.hpp"

#include <algorithm>
#include "Widgets/CentralPanelWidget.hpp"

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
}

App::App()
: _searchSpritesTask(std::bind(&App::OnSpritesFound, this, std::placeholders::_1))
{}

void App::Init()
{
    _rightWidget = std::make_unique<RightPanelWidget>();
    _rightWidget->Init();

    _centerWidget = std::make_unique<CentralPanelWidget>();
    _centerWidget->Init();

    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe<RightPanelActions::SearchSprites>(std::bind(&App::OnSearchSprites, this, std::placeholders::_1));

    auto saveSpritesCB = [this](const RightPanelActions::SaveFile&)
    {
        OnSaveFile();
    };
    broker.Subscribe<RightPanelActions::SaveFile>(saveSpritesCB);
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

    _centerWidget->Draw();
    ImGui::SameLine(0.0f, 0.0f);
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

        _foundSprites.reset();
    }
}

void App::OnSaveFile()
{
    std::string outFile;
    if (Platform::ShowSaveFileDialogue("Save Sprites", outFile, Serializer::GetSerializerFilters()))
    {
        Serializer::Serialize(outFile, *_foundSprites);

        AppConst::ReplaceExtension(outFile, "png");
        _openedImage->Save(outFile.c_str());
    }
}

void App::OnSearchSprites(const RightPanelActions::SearchSprites& action)
{
    {
        std::lock_guard<std::mutex> spriteList(_foundSpritesMutex);
        _foundSprites.reset();
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
    _searchSpritesTask.Run(callbacks, action.AlphaColor, static_cast<const void*>(_openedImage.get()));
}

void App::OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites)
{
    std::lock_guard<std::mutex> spriteList(_foundSpritesMutex);
    _foundSprites = std::make_shared<SpriteList>(foundSprites);

    MessageBroker::GetInstance().Broadcast(GenericActions::SpriteSearchFinished(_foundSprites));

    _searchingPopupState = PopupState::Close;
}

void App::OnCancelSearch()
{
    _searchSpritesTask.Stop();

    _searchingPopupState = PopupState::Close;
}
