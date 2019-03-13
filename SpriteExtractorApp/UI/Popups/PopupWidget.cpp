#include "PopupWidget.hpp"

PopupWidget::PopupWidget(const std::string &popupName, bool isCloseable)
: _popupName(popupName)
, _isCloseable(isCloseable)
{
}

void PopupWidget::Draw()
{
    bool canBeClosed = _isCloseable;
    if (ImGui::BeginPopupModal(_popupName.c_str(), &canBeClosed))
    {
        DrawPopup();
        ImGui::EndPopup();
    }
    else if (!canBeClosed && _isCloseable)
    {
        Close();
    }
}

void PopupWidget::Close()
{
    _state = State::Close;
}
