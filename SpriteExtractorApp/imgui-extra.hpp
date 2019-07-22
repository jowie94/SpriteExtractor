#pragma once

#include <string>

#include <ImGui/imgui.h>

struct ITextureResource;

namespace ImGui
{
    void Image(const ITextureResource& image, const ImVec2& imageSize);
    void Image(const ITextureResource& image, const ImVec2& imageSize, const ImVec2& uv0, const ImVec2& uv1);

    bool Button(const char* label, bool enabled);
    bool Button(const char* label, bool enabled, const ImVec2& size);

    bool InputTextWithHint(const char* label, const char* hint, std::string& str, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
}
