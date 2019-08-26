#pragma once

#include <string>

#include "Services/AssetManager/AssetLoader.hpp"

struct ImFont;

class ImFontAsset
{
public:
    ImFont* GetFont();

private:
    friend struct AssetLoader<ImFontAsset>;

    ImFontAsset(std::string&& fontPath);

    ImFont* _font = nullptr;
    std::string _fontPath;
    float _fontSize = 18.0f;
};

template<>
struct AssetLoader<ImFontAsset>
{
    static ImFontAsset* LoadAsset(const char* assetPath)
    {
        return new ImFontAsset(assetPath);
    }
};
