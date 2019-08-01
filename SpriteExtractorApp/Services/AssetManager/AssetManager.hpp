#pragma once

#include <unordered_map>
#include <memory>

#include "AssetLoader.hpp"

class AssetManager
{
public:
    template<typename Asset>
    AssetPtr<Asset> GetAsset(const std::string& assetName)
    {
        GenericPtr asset = FindAsset(assetName);

        if (!asset)
        {
            asset = GenericPtr(AssetLoader<Asset>::LoadAsset(assetName.c_str()));
            _cache.emplace(assetName, asset);
        }

        return std::static_pointer_cast<Asset>(asset);
    }

private:
    using GenericPtr = AssetPtr<void>;

    GenericPtr FindAsset(const std::string& assetName);

    std::unordered_map<std::string, GenericPtr> _cache;
};
