#pragma once

#include <memory>
#include <ImGui/imgui.h>

#include <UI/PanelWindow.hpp>

#include "Types.hpp"

class SpriteSheet;

namespace GenericActions 
{
    struct ImageOpened;
}

namespace RightPanelActions 
{
    struct ToggleColorPicker;
}

namespace SpriteSearchMessages
{
    struct SpriteSearchFinished;
}

class SpritesPanel : public PanelWindow
{
public:
	SpritesPanel();

    void Init() override;

protected:
    void Draw() override;

private:
    void OnToggleColorPicker(const RightPanelActions::ToggleColorPicker& toggle);
    void OnImageOpened(const GenericActions::ImageOpened& openedImage);

    void DrawImage();
    void DrawZoom();

	void DrawSpriteBoxes(const ImVec2& cursorScreenPosition);

    Color CalculateHoveredColor(const ImVec2& mousePosition, std::shared_ptr<const IImage> image);

	std::shared_ptr<SpriteSheet> _spriteSheet;
    std::unique_ptr<ITextureResource> _textureResource;

    float _imageScale = 1.0f;
    ImVec2 _imageWindowSize;
    bool _enableColorPicker = false;
};
