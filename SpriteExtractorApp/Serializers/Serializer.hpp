#pragma once

#include <string>

#include "Platform/GenericPlatform.h"
#include "SpriteExtractor.h"

class SpriteSheet;

namespace Serializer
{
    const std::vector<Platform::FileFilter>& GetSerializerFilters();
    void Serialize(const std::string& outFile, const SpriteSheet& sprites);
}
