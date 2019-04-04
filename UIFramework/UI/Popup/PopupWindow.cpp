#include "PopupWindow.hpp"

#include <ImGui/imgui.h>

PopupWindow::PopupWindow(const char* popupName, bool isCloseable)
: BaseWindow(popupName, ImGuiWindowFlags_None)
, _isCloseable(isCloseable)
{
}

void PopupWindow::Draw()
{
    bool* canBeClosed = _isCloseable ? &_isCloseable : nullptr;
    if (ImGui::BeginPopupModal(GetName(), canBeClosed))
    {
        _isDrawing = true;
        DrawPopup();
        ImGui::EndPopup();
        _isDrawing = false;
    }
    else if (canBeClosed && *canBeClosed)
    {
        Close();
    }
}

void PopupWindow::Close()
{
    if (_isDrawing)
    {
        ImGui::CloseCurrentPopup();
    }
    
    _state = State::Close;
}

void PopupWindow::BeginWidget()
{
}

void PopupWindow::EndWidget()
{
}
