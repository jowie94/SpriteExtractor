#pragma once

#include <string>

#include "Types.hpp"

struct Sprite
{
    int Idx = 0;
    std::string Name;
    BBox BoundingBox;
};
