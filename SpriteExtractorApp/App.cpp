#include "App.hpp"

#include <algorithm>
#include <ImGui/imgui.h>

#include "Platform/GenericPlatform.h"
#include "Serializers/Serializer.hpp"

#include "MessageBroker.hpp"
#include "Messages/GenericActions.hpp"
#include "Messages/RightPanelActions.hpp"
#include "Messages/MainWindowActions.hpp"
#include "Messages/SpriteSearchMessages.hpp"

#include "UI/Widgets/MainWindowWidget.hpp"

#include "Logger/Logger.hpp"

#include "imgui-extra.hpp"

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
	Logger::GetLogger("App")->info("Initializing App");
    _mainWindow = std::make_unique<MainWindowWidget>();
    _mainWindow->Init();

    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe<RightPanelActions::SearchSprites>(std::bind(&App::OnSearchSprites, this, std::placeholders::_1));

    auto saveSpritesCB = [this](const RightPanelActions::SaveFile&)
    {
        OnSaveFile();
    };
    broker.Subscribe<RightPanelActions::SaveFile>(saveSpritesCB);

    auto selectFileCB = [this](const MainWindowActions::SelectFile&)
    {
        OnSelectFile();
    };
    broker.Subscribe<MainWindowActions::SelectFile>(selectFileCB);

    broker.Subscribe<MainWindowActions::CancelSearch>(std::bind(&App::OnCancelSearch, this, std::placeholders::_1));
}

void App::Loop()
{
    if (_searchSpritesTask.IsRunning())
    {
        // TODO: Refactor
        SpriteSearchMessages::ProgressUpdate progressUpdate;
        progressUpdate.Stage = _searchSpritesTask.GetStage();
        progressUpdate.Progress = _searchSpritesTask.GetProgress();

        MessageBroker::GetInstance().Broadcast(progressUpdate);
    }

    _mainWindow->Draw();

    //ImGui::ShowTestWindow();
}

void App::OnSelectFile()
{
    if (Platform::ShowOpenFileDialogue("Choose an sprite sheet image", _selectedFile, AppConst::kImgFilter))
    {
        MessageBroker& broker = MessageBroker::GetInstance();

        _openedImage = OpenImage(_selectedFile);
        broker.Broadcast(GenericActions::ImageOpened(_selectedFile, _openedImage));

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

    _searchSpritesTask.Run(callbacks, action.AlphaColor, static_cast<const void*>(_openedImage.get()));
}

void App::OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites)
{
    std::lock_guard<std::mutex> spriteList(_foundSpritesMutex);
    _foundSprites = std::make_shared<SpriteList>(foundSprites);

    MessageBroker::GetInstance().Broadcast(SpriteSearchMessages::SpriteSearchFinished(_foundSprites));
}

void App::OnCancelSearch(const MainWindowActions::CancelSearch& /*cancelSearch*/)
{
    _searchSpritesTask.Stop();
}
