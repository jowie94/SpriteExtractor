#pragma once

#include "UI/Popup/PopupWindow.hpp"

#include <memory>

#include "Model/SpriteSheet/Animation.hpp"

struct ITextureResource;

class NewAnimationPopup : public PopupWindow
{
public:
	NewAnimationPopup();

	void Init() override;
	void BeforeDraw() override;
	void Draw() override;

private:
	void DrawAnimation();

	size_t _currentFrame = 0;
	float _elapsed = 0.0f;
	
	std::unique_ptr<ITextureResource> _texture;
	Animation _animation;
};
