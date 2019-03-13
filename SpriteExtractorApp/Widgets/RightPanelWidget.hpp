#pragma once

#include "IWidget.hpp"

#include "Types.hpp"

namespace GenericActions 
{
    struct ImageOpened;
    struct OpenImage;
    struct ColorHovered;
    struct ColorPicked;
}

namespace SpriteSearchMessages
{
    struct SpriteSearchFinished;
}

class RightPanelWidget : public IWidget
{
public:
    ~RightPanelWidget() override = default;

    void Init() override;
    void Draw() override;

private:
    void OnImageOpened(const GenericActions::ImageOpened& openedImage);
    void OnColorHovered(const GenericActions::ColorHovered& colorHovered);
    void OnColorPicked(const GenericActions::ColorPicked& colorPicked);
    void OnSpriteSearchFinished(const SpriteSearchMessages::SpriteSearchFinished& foundSprites);

    Color _alphaColor;

    bool _imageIsOpened = false;
    bool _enableColorPicker = false;
    bool _hasSprites = false;
};
