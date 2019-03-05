#pragma once

#include <string>
#include "SpriteExtractor.h"

namespace Paper2DSerializer
{
    void Serialize(const std::string& outputFile, const SpriteExtractor::SpriteList& spriteList);
}
