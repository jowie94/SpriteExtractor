#pragma once

#include <memory>

template<typename T>
using AssetPtr = std::shared_ptr<T>;

template<typename T>
using WeakAssetPtr = std::weak_ptr<T>;

template<typename Asset>
struct AssetLoader
{
    static Asset* LoadAsset(const char* assetPath)
    {
        static_assert(false, "Unrecognized asset type");

        return nullptr;
    }
};
