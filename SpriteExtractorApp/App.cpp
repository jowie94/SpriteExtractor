#include "App.hpp"

#include <algorithm>
#include <experimental/filesystem>

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
#include "Model/SpriteSheet.hpp"

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

App::~App() = default;

void App::Init()
{
    Logger::GetLogger("App")->info("Initializing App");
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

	_commandQueue.Update();

    _mainWindow->DoDraw();
    //ImGui::ShowTestWindow();
}

void App::OnSelectFile()
{
    auto logger = Logger::GetLogger("App");
    if (Platform::ShowOpenFileDialogue("Choose an sprite sheet image", _selectedFile, AppConst::kImgFilter))
    {
        MessageBroker& broker = MessageBroker::GetInstance();

		// TODO
		ModelManager& manager = ModelManager::GetInstance();
		manager.Remove<SpriteSheet>();

        _openedImage = OpenImage(_selectedFile);

        if (_openedImage)
        {
			manager.Create<SpriteSheet>();
			_commandQueue.Clear();

            broker.Broadcast(GenericActions::ImageOpened(_selectedFile, _openedImage));
            logger->info("Opened image {}", _selectedFile);
        }
        else
        {
            logger->error("Error opening image {}", _selectedFile);
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
        Serializer::Serialize(outFile, *ModelManager::GetInstance().Get<SpriteSheet>());

        AppConst::ReplaceExtension(outFile, "png");
        if(!_openedImage->Save(outFile.c_str()))
        {
            logger->error("Couldn't save {}", outFile);
        }
        else
        {
            logger->info("Successfully saved {}", outFile);
        }
    }
}

void App::OnSearchSprites(const RightPanelActions::SearchSprites& action)
{
    {
        std::lock_guard<std::mutex> spriteList(_foundSpritesMutex);
        //_foundSprites.reset();
    }

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
    _searchSpritesTask.Run(callbacks, action.AlphaColor, static_cast<const void*>(_openedImage.get()));
}

void App::OnSpritesFound(const SpriteExtractor::SpriteList& foundSprites)
{
    std::lock_guard<std::mutex> spriteList(_foundSpritesMutex);
    //_foundSprites = std::make_shared<SpriteSearchMessages::SpriteList>(foundSprites);

	// TODO
	std::vector<Sprite> sprites;
	sprites.reserve(foundSprites.size());

	std::experimental::filesystem::path filePath(_selectedFile);
	std::string fileName = filePath.stem().string();
	size_t last = 0;
	for (const auto& foundSprite : foundSprites)
	{
		Sprite sprite;
		sprite.BoundingBox = foundSprite;
		sprite.Name = fileName + "_" + std::to_string(last);
		++last;

		sprites.emplace_back(std::move(sprite));
	}

	MessageBroker& broker = MessageBroker::GetInstance();
	broker.Broadcast(CommandQueue::PushCommandMessage(std::make_shared<SpriteSheet::UpdateSpritesCommand>(sprites)));

    Logger::GetLogger("Extract task")->info("Finished searching sprites. {} sprites found", foundSprites.size());
    broker.Broadcast(SpriteSearchMessages::SpriteSearchFinished(foundSprites.size()));
}

void App::OnCancelSearch(const MainWindowActions::CancelSearch& /*cancelSearch*/)
{
    _searchSpritesTask.Stop();
}
