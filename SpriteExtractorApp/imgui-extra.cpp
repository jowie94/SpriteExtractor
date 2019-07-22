#include "imgui-extra.hpp"
#include <ImGui/imgui.h>
#include "Types.hpp"

namespace ImGui
{
    namespace Helpers
    {
        struct InputStringCustomData
        {
            std::string* String = nullptr;
            ImGuiInputTextCallback ChainCallback = nullptr;
            void* ChainUserData = nullptr;
        };

        int InputStringCallback(ImGuiInputTextCallbackData* data)
        {
            InputStringCustomData* user_data = static_cast<InputStringCustomData*>(data->UserData);
            if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
            {
                // Resize string callback
                // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
                std::string* str = user_data->String;
                IM_ASSERT(data->Buf == str->c_str());
                str->resize(data->BufTextLen);
                data->Buf = const_cast<char*>(str->c_str());
            }
            else if (user_data->ChainCallback)
            {
                // Forward to user callback, if any
                data->UserData = user_data->ChainUserData;
                return user_data->ChainCallback(data);
            }

            return 0;
        }
    }
}

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

bool ImGui::InputTextWithHint(const char* label, const char* hint, std::string& str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags& ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    Helpers::InputStringCustomData customData;
    customData.String = &str;
    customData.ChainCallback = callback;
    customData.ChainUserData = user_data;

    return InputTextWithHint(label, hint, const_cast<char*>(str.c_str()), str.capacity() + 1, flags, Helpers::InputStringCallback, &customData);
}
