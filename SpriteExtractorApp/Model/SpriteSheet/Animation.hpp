#pragma once

#include <string_view>
#include <vector>

struct Animation
{
    std::string_view Name;
    std::vector<int> SpriteIds;
};
