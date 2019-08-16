#pragma once

#include <memory>

#include <Utils/Traits.hpp>

template<typename T>
using AssetPtr = std::shared_ptr<T>;

template<typename T>
using WeakAssetPtr = std::weak_ptr<T>;

template<typename Asset>
struct AssetLoader
{
    static Asset* LoadAsset(const char* /*assetPath*/)
    {
        static_assert(Traits::always_false_v<Asset>, "Unrecognized asset type");

        return nullptr;
    }
};
