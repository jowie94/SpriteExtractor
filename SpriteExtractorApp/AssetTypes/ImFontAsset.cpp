#include "ImFontAsset.hpp"

#include <ImGui/imgui.h>
#include "imgui-sfml/imgui-SFML.h"

ImFont* ImFontAsset::GetFont()
{
    return _font;
}

ImFontAsset::ImFontAsset(std::string&& fontPath)
{
    size_t commaPos = fontPath.find(',');

    if (commaPos != std::string::npos)
    {
        _fontPath = fontPath.substr(0, commaPos);
        _fontSize = std::strtof(fontPath.substr(commaPos + 1).c_str(), nullptr);
    }
    else
    {
        _fontPath = std::move(fontPath);
        _fontSize = 18.0f;
    }

    _font = ImGui::GetIO().Fonts->AddFontFromFileTTF(_fontPath.c_str(), _fontSize);
    ImGui::SFML::UpdateFontTexture();
}
