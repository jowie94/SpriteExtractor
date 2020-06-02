#pragma once

#include <memory>

template<typename T>
using AssetPtr = std::shared_ptr<T>;

template<typename T>
using WeakAssetPtr = std::weak_ptr<T>;
