#include "PopupWindow.hpp"

#include <ImGui/imgui.h>

PopupWindow::PopupWindow(const char* popupName, bool isCloseable)
: BaseWindow(popupName, ImGuiWindowFlags_None)
, _isCloseable(isCloseable)
{
}

void PopupWindow::Close()
{
    if (_isDrawing)
    {
        ImGui::CloseCurrentPopup();
    }
    
    _state = State::Close;
}

bool PopupWindow::BeginWidget()
{
    bool* opened = _isCloseable ? &_opened : nullptr;
    _isDrawing = ImGui::BeginPopupModal(GetName(), opened);
    return _isDrawing;
}

void PopupWindow::EndWidget(bool wasDrawn)
{
    if (wasDrawn)
    {
        ImGui::EndPopup();
        _isDrawing = false;
    }
    else if (_isCloseable && !_opened)
    {
        Close();
    }
}
