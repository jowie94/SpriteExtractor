#pragma once

#include "IWidget.hpp"

#include "Types.hpp"

namespace GenericActions 
{
    struct ImageOpened;
    struct OpenImage;
}

class RightPanelWidget : public IWidget
{
public:
    ~RightPanelWidget() override = default;

    void Init() override;
    void Draw() override;

private:
    void OnOpenImage(const GenericActions::OpenImage& image);
    void OnImageOpened(const GenericActions::ImageOpened& openedImage);

    Color _alphaColor;
    Color _originalAlphaColor;

    bool _imageIsOpened = false;
    bool _enableColorPicker = false;
    bool _hasSprites = false;
};
