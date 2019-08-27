#include "AssetManager.hpp"

#include "Services/Services.hpp"

REGISTER_SERVICE(AssetManager);

AssetManager::GenericPtr AssetManager::FindAsset(const std::string& assetName)
{
    auto it = _cache.find(assetName);

    return it != _cache.end() ? it->second.lock() : nullptr;
}
