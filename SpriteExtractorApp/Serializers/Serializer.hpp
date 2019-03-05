#pragma once

#include <string>
#include "Platform/GenericPlatform.h"
#include "SpriteExtractor.h"

namespace Serializer
{
    const std::vector<Platform::FileFilter>& GetSerializerFilters();
    void Serialize(const std::string& outFile, const SpriteExtractor::SpriteList& sprites);
}
