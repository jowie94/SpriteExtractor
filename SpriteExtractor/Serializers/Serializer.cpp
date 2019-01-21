#include "Serializer.hpp"
#include "Paper2DSerializer.hpp"

#include <unordered_map>

namespace Serializer
{
    static const std::vector<Platform::FileFilter> kSerializerFilter =
    {
        { "Unreal Paper2D", "*.paper2dsprites" }
    };

    enum class Serializers
    {
        None,
        Paper2DSprite
    };

    static const std::unordered_map<std::string, Serializers> kExtensionToSerializer =
    {
        {".paper2dsprites", Serializers::Paper2DSprite}
    };

    std::string GetFileExtension(const std::string& fileName)
    {
        size_t dotPos = fileName.find_last_of('.');
        if (dotPos != std::string::npos)
        {
            return fileName.substr(dotPos);
        }

        return "";
    }
}

const std::vector<Platform::FileFilter>& Serializer::GetSerializerFilters()
{
    return kSerializerFilter;
}

void Serializer::Serialize(const std::string& outFile, const SpriteExtractor::SpriteList& sprites)
{
    std::string extension = GetFileExtension(outFile);

    auto serializerIt = kExtensionToSerializer.find(extension);

    assert(serializerIt != kExtensionToSerializer.end());
    if (serializerIt == kExtensionToSerializer.end())
    {
        return;
    }

    switch (serializerIt->second)
    {
        case Serializers::Paper2DSprite:
        {
            Paper2DSerializer::Serialize(outFile, sprites);
            break;
        }
        default:
        {
            assert(false && "Unknown serializer type");
        }
    }
}
