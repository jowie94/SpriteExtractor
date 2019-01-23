#pragma once
#include <string>
#include <utility>
#include "Types.hpp"

namespace GenericActions
{
    struct ImageOpened
    {
        ImageOpened(std::weak_ptr<IImage> openedImage)
        : OpenedImage(std::move(openedImage))
        {}

        std::weak_ptr<IImage> OpenedImage;
    };

    struct ColorHovered
    {
        Color HoveredColor;
    };

    struct ColorPicked
    {
        Color PickedColor;
    };

    struct SpriteSearchFinished
    {
        SpriteSearchFinished(std::vector<BBox>& foundSprites)
        : FoundSprites(foundSprites)
        {}

        std::vector<BBox>& FoundSprites;
    };
}
