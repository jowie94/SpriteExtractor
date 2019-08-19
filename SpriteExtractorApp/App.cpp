#include "App.hpp"

#include <algorithm>
#include <experimental/filesystem>
#include <string_view>

#include <ImGui/imgui.h>

#include "Platform/GenericPlatform.h"
#include "Serializers/Serializer.hpp"

#include "MessageBroker.hpp"
#include "Messages/GenericActions.hpp"
#include "Messages/RightPanelActions.hpp"
#include "Messages/MainWindowActions.hpp"
#include "Messages/SpriteSearchMessages.hpp"

#include "Windows/MainWindow.hpp"

#include "Logger/Logger.hpp"
#include "Model/ModelManager.hpp"
#include "Model/SpriteSheet/SpriteSheet.hpp"
#include "Model/SpriteSheet/SpriteSheetActions.hpp"

#include "Services/Services.hpp"
#include "Services/AssetManager/AssetManager.hpp"
#include "Services/Scheduler/Scheduler.hpp"

#include "AssetTypes/FontAsset.hpp"

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

    std::string_view GetFile(std::string_view path)
    {
        std::string_view::size_type pos = path.rfind(std::experimental::filesystem::path::preferred_separator);
        pos = pos != std::string_view::npos ? pos + 1 : 0;

        return path.substr(pos);
    }

    std::string_view GetFileName(std::string_view file)
    {
        std::string_view fileName = GetFile(file);

        std::string_view::size_type pos = fileName.rfind('.');
        pos = pos != std::string_view::npos ? pos : fileName.size();

        return fileName.substr(0, pos);
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

App::~App() = default;

void App::Init()
{
    Logger::GetLogger("App")->info("Initializing App");

    Services::GetInstance().InitServices();

    _mainWindow = std::make_unique<MainWindow>();
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

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.IniFilename = nullptr;

    AssetPtr<FontAsset> noto = Services::GetInstance().Get<AssetManager>()->GetAsset<FontAsset>("resources/noto-sans-bold.ttf");
    noto->SetFontSize(18.0f);
    io.FontDefault = noto->GetFont();
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

    Services::GetInstance().Get<Scheduler>()->Update();

    _mainWindow->DoDraw();
    //ImGui::ShowTestWindow();
}

void App::OnSelectFile()
{
    auto logger = Logger::GetLogger("App");
    std::string selectedFile;
    if (Platform::ShowOpenFileDialogue("Choose an sprite sheet image", selectedFile, AppConst::kImgFilter))
    {
        MessageBroker& broker = MessageBroker::GetInstance();

        _currentSpriteSheet.reset();

        // TODO
        ModelManager& manager = ModelManager::GetInstance();
        manager.Remove<SpriteSheet>();

        std::shared_ptr<IImage> openedImage = OpenImage(selectedFile);

        if (openedImage)
        {
            _currentSpriteSheet = manager.Create<SpriteSheet>(openedImage, selectedFile);
            _commandQueue.Clear();

            broker.Broadcast(GenericActions::ImageOpened());
            logger->info("Opened image {}", selectedFile);
        }
        else
        {
            logger->error("Error opening image {}", selectedFile);
        }
    }
    else
    {
        logger->error("Couldn't open file");
    }
}

void App::OnSaveFile()
{
    std::string outFile;
    if (Platform::ShowSaveFileDialogue("Save Sprites", outFile, Serializer::GetSerializerFilters()))
    {
        auto logger = Logger::GetLogger("App");
        logger->info("Serializing {}", outFile);

        // TODO
        Serializer::Serialize(outFile, *_currentSpriteSheet);

        AppConst::ReplaceExtension(outFile, "png");

        if (auto image = _currentSpriteSheet->GetImage().lock())
        {
            if (image->Save(outFile.c_str()))
            {
                logger->info("Successfully saved {}", outFile);
            }
            else
            {
                logger->error("Couldn't save {}", outFile);
            }
        }
    }
}

void App::OnSearchSprites(const RightPanelActions::SearchSprites& action)
{
    _commandQueue.Clear();

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

    Logger::GetLogger("Extract task")->info("Starting extraction");
    _searchSpritesTask.Run(callbacks, _currentSpriteSheet->GetAlphaColor(), static_cast<const void*>(_currentSpriteSheet->GetImage().lock().get()));
}

void App::OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites)
{
    // TODO
    std::vector<std::shared_ptr<Sprite>> sprites;
    sprites.reserve(foundSprites.size());

    std::string_view fileName = AppConst::GetFileName(_currentSpriteSheet->GetImageName());
    size_t last = 0;
    for (const auto& foundSprite : foundSprites)
    {
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();
        sprite->Idx = last;
        sprite->BoundingBox = foundSprite;
        sprite->Name = std::string(fileName) + "_" + std::to_string(last); // TODO: Improve
        ++last;

        sprites.emplace_back(std::move(sprite));
    }

    MessageBroker& broker = MessageBroker::GetInstance();
    broker.Broadcast(Commands::PushCommandMessage(std::make_shared<Commands::Model::UpdateSpritesCommand>(sprites)));

    Logger::GetLogger("Extract task")->info("Finished searching sprites. {} sprites found", foundSprites.size());
    broker.Broadcast(SpriteSearchMessages::SpriteSearchFinished(foundSprites.size()));
}

void App::OnCancelSearch(const MainWindowActions::CancelSearch& /*cancelSearch*/)
{
    _searchSpritesTask.Stop();
}
