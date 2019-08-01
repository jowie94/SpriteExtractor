#pragma once

#include <string>

#include "Services/AssetManager/AssetLoader.hpp"

struct ImFont;

class FontAsset
{
public:
    ImFont* GetFont();

    void SetFontSize(float size);

private:
    friend struct AssetLoader<FontAsset>;

    FontAsset(std::string&& fontPath);

    void Initialize();

    ImFont* _font = nullptr;
    std::string _fontPath;
    float _fontSize = 10.0f;
};

template<>
struct AssetLoader<FontAsset>
{
    static FontAsset* LoadAsset(const char* assetPath)
    {
        return new FontAsset(assetPath);
    }
};
