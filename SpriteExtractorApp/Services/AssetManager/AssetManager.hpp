#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

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
            std::function<void(Asset*)> deleter = std::bind(&AssetManager::DeleteAsset<Asset>, this, assetName, std::placeholders::_1);
            asset = GenericPtr(AssetLoader<Asset>::LoadAsset(assetName.c_str()), deleter);
            _cache.emplace(assetName, asset);
        }

        return std::static_pointer_cast<Asset>(asset);
    }

private:
    using GenericWeakPtr = std::weak_ptr<void>;
    using GenericPtr = std::shared_ptr<void>;

    template<typename Asset>
    void DeleteAsset(std::string name, Asset* ptr)
    {
        delete ptr;
        _cache.erase(name);
    }

    GenericPtr FindAsset(const std::string& assetName);

    std::unordered_map<std::string, GenericWeakPtr> _cache;
};
