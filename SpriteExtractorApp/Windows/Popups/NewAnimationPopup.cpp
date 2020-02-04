#include "NewAnimationPopup.hpp"

#include "MessageBroker.hpp"

#include "Model/SpriteSheet/SpriteSheetActions.hpp"

NewAnimationPopup::NewAnimationPopup()
: PopupWindow("New Animation", true)
{
	
}

void NewAnimationPopup::Init()
{
	PopupWindow::Init();
}

void NewAnimationPopup::Draw()
{
	PopupWindow::Draw();

	ImGui::Text("TMP");

	if (ImGui::Button("Test"))
	{
		std::vector<std::string> names = { "test_1", "test_2", "test_5" };
		std::shared_ptr<Commands::Model::CreateAnimationCommand> command = std::make_shared<Commands::Model::CreateAnimationCommand>("asdf", names);
		Commands::PushCommandMessage message(command);
		MessageBroker::GetInstance().Broadcast(message);
	}
}
