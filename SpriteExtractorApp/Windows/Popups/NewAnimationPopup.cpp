#include "NewAnimationPopup.hpp"

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
}
