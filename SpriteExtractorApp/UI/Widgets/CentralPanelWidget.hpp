#pragma once

#include <memory>
#include <imgui.h>

#include "IWidget.hpp"
#include "Types.hpp"

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

class CentralPanelWidget : public IWidget
{
public:
    void Init() override;
    void Draw() override;

private:
    void OnToggleColorPicker(const RightPanelActions::ToggleColorPicker& toggle);
    void OnImageOpened(const GenericActions::ImageOpened& openedImage);
    void OnSpritesFound(const SpriteSearchMessages::SpriteSearchFinished& spritesFound);

    void DrawImage();
    void DrawZoom();

    Color CalculateHoveredColor(const ImVec2& mousePosition, const std::shared_ptr<IImage> image);

    std::weak_ptr<IImage> _openedImage;
    std::weak_ptr<SpriteList> _foundSprites;
    std::unique_ptr<ITextureResource> _textureResource;

    float _imageScale = 1.0f;
    ImVec2 _imageWindowSize;
    bool _enableColorPicker = false;
};
