#pragma once

#include <QPixmap>

#include "Services/AssetManager/AssetLoader.hpp"

template <>
struct AssetLoader<QPixmap>
{
    static QPixmap* LoadAsset(const char* assetPath)
    {
        return new QPixmap(assetPath);
    }
};
