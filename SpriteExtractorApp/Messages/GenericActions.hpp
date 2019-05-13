#pragma once

#include "Types.hpp"

namespace GenericActions
{
    struct ImageOpened {};

    struct ColorHovered
    {
        Color HoveredColor;
    };

    struct ColorPicked
    {
        Color PickedColor;
    };
}
