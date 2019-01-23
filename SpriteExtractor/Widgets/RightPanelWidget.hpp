#pragma once

#include "IWidget.hpp"

#include "Types.hpp"

namespace GenericActions 
{
    struct SpriteSearchFinished;
    struct ImageOpened;
    struct OpenImage;
    struct ColorHovered;
    struct ColorPicked;
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
    void OnSpriteSearchFinished(const GenericActions::SpriteSearchFinished& foundSprites);

    Color _alphaColor;

    bool _imageIsOpened = false;
    bool _enableColorPicker = false;
    bool _hasSprites = false;
};
