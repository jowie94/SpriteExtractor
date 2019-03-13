#include "PopupsController.hpp"

#include <imgui.h>

#include "Widgets/IWidget.hpp"

void PopupsController::ShowPopup(std::shared_ptr<PopupWidget> popup)
{
    popup->_state = PopupWidget::State::Open;
    _popupQueue.emplace(std::move(popup));
}

void PopupsController::Draw()
{
    if (!_popupQueue.empty())
    {
        std::shared_ptr<PopupWidget> popup = _popupQueue.front();

        if (popup->_state == PopupWidget::State::Open)
        {
            ImGui::OpenPopup(popup->_popupName.c_str());
            popup->_state = PopupWidget::State::Opened;
        }

        popup->Draw();

        if (popup->_state == PopupWidget::State::Close)
        {
            popup->_state = PopupWidget::State::Closed;
            ImGui::CloseCurrentPopup();
            _popupQueue.pop();
        }
    }
}
