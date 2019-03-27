#include "PopupWidget.hpp"

#include <ImGui/imgui.h>

PopupWidget::PopupWidget(const char* popupName, bool isCloseable)
: IWidget(popupName, ImGuiWindowFlags_None)
, _isCloseable(isCloseable)
{
}

void PopupWidget::Draw()
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

void PopupWidget::Close()
{
    if (_isDrawing)
    {
        ImGui::CloseCurrentPopup();
    }
    
    _state = State::Close;
}

void PopupWidget::BeginWidget()
{
}

void PopupWidget::EndWidget()
{
}
