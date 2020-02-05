#pragma once

#include <string>
#include <memory>

#include <ImGui/imgui.h>

struct BBox;
struct ITextureResource;

namespace ImGui
{
    void Image(const ITextureResource& image, const ImVec2& imageSize);
    void Image(const ITextureResource& image, const ImVec2& imageSize, const BBox& box);
    void Image(const ITextureResource& image, const ImVec2& imageSize, const ImVec2& uv0, const ImVec2& uv1);

    void SpriteFrame(const char* name, const std::unique_ptr<ITextureResource>& spriteSheet, const BBox& spriteRect, const ImVec2& size = ImVec2(0.0f, 0.0f), bool border = false);
	
    bool Button(const char* label, bool enabled);
    bool Button(const char* label, bool enabled, const ImVec2& size);

    bool InputTextWithHint(const char* label, const char* hint, std::string& str, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
}
