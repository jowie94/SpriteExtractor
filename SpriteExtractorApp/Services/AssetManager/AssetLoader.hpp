#pragma once

#include <Utils/Traits.hpp>

template<typename Asset>
struct AssetLoader
{
    static Asset* LoadAsset(const char* /*assetPath*/)
    {
        static_assert(Traits::always_false_v<Asset>, "Unrecognized asset type");

        return nullptr;
    }
};
