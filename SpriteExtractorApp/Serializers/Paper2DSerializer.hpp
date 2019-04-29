#pragma once

#include <string>

class SpriteSheet;

namespace Paper2DSerializer
{
    void Serialize(const std::string& outputFile, const SpriteSheet& spriteList);
}
