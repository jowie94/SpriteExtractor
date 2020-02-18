#include "NewAnimationPopup.hpp"

#include "MessageBroker.hpp"
#include "Logger/Logger.hpp"

#include "Model/SpriteSheet/SpriteSheetActions.hpp"

#include "imgui-extra.hpp"
#include "Services/Services.hpp"
#include "Services/Scheduler/Scheduler.hpp"

namespace NewAnimationPopupConst
{
	const ImVec2 kSpriteSize(50.0f, 50.0f);
	const float kSpacing = 3.0f;
}

NewAnimationPopup::NewAnimationPopup()
: PopupWindow("New Animation", true)
{
	
}

void NewAnimationPopup::Init()
{
	PopupWindow::Init();

	// TODO
	_texture = ModelManager::GetInstance().Get<SpriteSheet>()->GetImage().lock()->GetTextureResource();
}

void NewAnimationPopup::BeforeDraw()
{
	ImGui::SetNextWindowSize(ImVec2(800.0f, 600.0f), ImGuiCond_Once);
}

void NewAnimationPopup::Draw()
{
	using namespace	NewAnimationPopupConst;
	
	PopupWindow::Draw();

	ImVec2 windowSize = ImGui::GetWindowSize();
	if (ImGui::BeginChild("Sprites", ImVec2(windowSize.x * 0.7f, 0), true))
	{
		// TODO: Store on init
		const std::vector<std::shared_ptr<Sprite>>& sprites = ModelManager::GetInstance().Get<SpriteSheet>()->GetSprites();

		const int spritesInLine = static_cast<int>(std::floorf(ImGui::GetWindowSize().x / (kSpriteSize.x + kSpacing))) - 1;

		int spritesDrawn = 0;
		for (const auto& sprite : sprites)
		{
			if (spritesDrawn > 0 && spritesDrawn < spritesInLine)
			{
				ImGui::SameLine(0.0f, kSpacing);
			}
			else
			{
				spritesDrawn = 0;
			}

			std::optional<ImColor> borderColor;
			auto animationIt = std::find(_animation.SpriteIds.begin(), _animation.SpriteIds.end(), sprite->Idx);
			bool isSelected = animationIt != _animation.SpriteIds.end();
			if (isSelected)
			{
				borderColor = ImColor(255.0f, 0.0f, 0.0f);
			}
			
			ImGui::SpriteFrame(sprite->Name.c_str(), _texture, sprite->BoundingBox, kSpriteSize, true, borderColor);
			if(ImGui::IsItemClicked())
			{
				Logger::GetLogger("Test")->info("Clicked {}", sprite->Name);
				

				if(isSelected)
				{
					_animation.SpriteIds.erase(animationIt);
				}
				else
				{
					_animation.SpriteIds.push_back(sprite->Idx);
				}

				_currentFrame = 0LL;
			}
			++spritesDrawn;
		}

	}
	ImGui::EndChild();
	ImGui::SameLine();
	DrawAnimation();
		
	if (ImGui::Button("Test"))
	{
		std::vector<std::string> names = { "test_1", "test_2", "test_5" };
		std::shared_ptr<Commands::Model::CreateAnimationCommand> command = std::make_shared<Commands::Model::CreateAnimationCommand>("asdf", names);
		Commands::PushCommandMessage message(command);
		MessageBroker::GetInstance().Broadcast(message);
	}
}

void NewAnimationPopup::DrawAnimation()
{
	constexpr float kSpF = 0.15f; // TODO: Parameter

	if (_animation.SpriteIds.empty())
	{
		return;
	}
	
	_elapsed += Services::GetInstance().Get<Scheduler>()->DeltaTime();
	size_t currentFrame = static_cast<size_t>(std::floor(_elapsed / kSpF)) % _animation.SpriteIds.size();

	int idx = _animation.SpriteIds.at(currentFrame);
	// TODO: Sprite idx != sprite position
	const std::vector<std::shared_ptr<Sprite>>& sprites = ModelManager::GetInstance().Get<SpriteSheet>()->GetSprites();
	const std::shared_ptr<Sprite> sprite = sprites.at(idx);
	ImGui::SpriteFrame(sprite->Name.c_str(), _texture, sprite->BoundingBox, ImVec2(100.0f, 100.0f), true);
}
