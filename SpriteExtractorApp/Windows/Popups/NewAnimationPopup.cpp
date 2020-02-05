#include "NewAnimationPopup.hpp"

#include "MessageBroker.hpp"
#include "Logger/Logger.hpp"

#include "Model/SpriteSheet/SpriteSheetActions.hpp"

#include "imgui-extra.hpp"

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

			ImGui::SpriteFrame(sprite->Name.c_str(), _texture, sprite->BoundingBox, kSpriteSize, true);
			if(ImGui::IsItemClicked())
			{
				Logger::GetLogger("Test")->info("Clicked {}", sprite->Name);
			}
			++spritesDrawn;
		}

	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::Button("Test"))
	{
		std::vector<std::string> names = { "test_1", "test_2", "test_5" };
		std::shared_ptr<Commands::Model::CreateAnimationCommand> command = std::make_shared<Commands::Model::CreateAnimationCommand>("asdf", names);
		Commands::PushCommandMessage message(command);
		MessageBroker::GetInstance().Broadcast(message);
	}
}
