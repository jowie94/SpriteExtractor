#include "imgui-extra.hpp"
#include <ImGui/imgui.h>
#include "Types.hpp"

void ImGui::Image(const ITextureResource& image, const ImVec2& imageSize)
{
    Image(image, imageSize, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
}

void ImGui::Image(const ITextureResource& image, const ImVec2& imageSize, const ImVec2& uv0, const ImVec2& uv1)
{
    Image((void*)(intptr_t)image.ResourceId, imageSize, uv0, uv1);
}

bool ImGui::Button(const char* label, bool enabled)
{
    return Button(label, enabled, ImVec2(0.0f, 0.0f));
}

bool ImGui::Button(const char* label, bool enabled, const ImVec2& size)
{
    if (!enabled)
    {
        PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Button));
        PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_Button));
    }

    bool result = Button(label, size);

    if (!enabled)
    {
        PopStyleVar();
        PopStyleColor(2);
    }

    return enabled && result;
}


