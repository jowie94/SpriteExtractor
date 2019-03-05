#pragma once

#include <memory>
#include <imgui.h>

#include "IWidget.hpp"
#include "Types.hpp"

namespace GenericActions 
{
    struct SpriteSearchFinished;
    struct ImageOpened;
}

namespace RightPanelActions 
{
    struct ToggleColorPicker;
}

class CentralPanelWidget : public IWidget
{
public:
    void Init() override;
    void Draw() override;

private:
    void OnToggleColorPicker(const RightPanelActions::ToggleColorPicker& toggle);
    void OnImageOpened(const GenericActions::ImageOpened& openedImage);
    void OnSpritesFound(const GenericActions::SpriteSearchFinished& spritesFound);

    std::weak_ptr<IImage> _openedImage;
    std::weak_ptr<SpriteList> _foundSprites;
    std::unique_ptr<ITextureResource> _textureResource;

    float _imageScale = 1.0f;
    ImVec2 _imageWindowSize;
    bool _enableColorPicker = false;
    Color _alphaColor;
    Color _originalAlphaColor;
};
