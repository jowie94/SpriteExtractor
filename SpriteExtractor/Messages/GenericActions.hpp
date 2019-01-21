#pragma once
#include <string>
#include <utility>
#include "Types.hpp"

namespace GenericActions
{
    struct OpenImage
    {
        OpenImage(std::string fileName)
        : FileName(std::move(fileName))
        {}

        std::string FileName;
    };

    struct ImageOpened
    {
        ImageOpened(std::weak_ptr<IImage> openedImage)
        : OpenedImage(std::move(openedImage))
        {}

        std::weak_ptr<IImage> OpenedImage;
    };
}
