#pragma once

#include "UI/Popup/PopupWindow.hpp"

class NewAnimationPopup : public PopupWindow
{
public:
	NewAnimationPopup();

	void Init() override;
	void Draw() override;
};
