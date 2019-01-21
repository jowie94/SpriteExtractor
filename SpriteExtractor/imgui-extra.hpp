#pragma once

struct ITextureResource;
struct ImVec2;

namespace ImGui
{
    void Image(const ITextureResource& image, const ImVec2& imageSize);

    bool Button(const char* label, bool enabled);
    bool Button(const char* label, bool enabled, const ImVec2& size);
}
