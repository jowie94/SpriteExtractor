#pragma once

#include "UI/Popup/PopupWindow.hpp"

#include <memory>

struct ITextureResource;

class NewAnimationPopup : public PopupWindow
{
public:
	NewAnimationPopup();

	void Init() override;
	void BeforeDraw() override;
	void Draw() override;

private:
	std::unique_ptr<ITextureResource> _texture;
};
