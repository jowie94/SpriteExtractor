#pragma once
#include <string>
#include <utility>
#include "Types.hpp"

namespace GenericActions
{
    struct ImageOpened
    {
        ImageOpened(const std::string& imageName, std::weak_ptr<IImage> openedImage)
        : ImageName(imageName)
        , OpenedImage(std::move(openedImage))
        {}

        std::string ImageName;
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
}
