#include "FontAsset.hpp"

#include <ImGui/imgui.h>
#include "imgui-sfml/imgui-SFML.h"

ImFont* FontAsset::GetFont()
{
    return _font;
}

void FontAsset::SetFontSize(float size)
{
    assert(!_font && "Font already initialized");
    _fontSize = size;

    Initialize();
}

FontAsset::FontAsset(std::string&& fontPath)
: _fontPath(std::move(fontPath))
{
}

void FontAsset::Initialize()
{
    _font = ImGui::GetIO().Fonts->AddFontFromFileTTF(_fontPath.c_str(), _fontSize);
    ImGui::SFML::UpdateFontTexture();
}
