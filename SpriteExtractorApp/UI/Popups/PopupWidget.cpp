#include "PopupWidget.hpp"

PopupWidget::PopupWidget(const std::string &popupName, bool isCloseable)
: _popupName(popupName)
, _isCloseable(isCloseable)
{
}

void PopupWidget::Draw()
{
    bool* canBeClosed = _isCloseable ? &_isCloseable : nullptr;
    if (ImGui::BeginPopupModal(_popupName.c_str(), canBeClosed))
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
