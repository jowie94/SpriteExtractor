#include "PopupsController.hpp"

#include <ImGui/imgui.h>

#include "UI/Popup/PopupWindow.hpp"

void PopupsController::ShowPopup(std::shared_ptr<PopupWindow> popup)
{
    popup->_state = PopupWindow::State::Open;
    _popupQueue.emplace(std::move(popup));
}

void PopupsController::Draw()
{
    if (!_popupQueue.empty())
    {
        std::shared_ptr<PopupWindow> popup = _popupQueue.front();

        if (popup->_state == PopupWindow::State::Open)
        {
            ImGui::OpenPopup(popup->GetName());
            popup->_state = PopupWindow::State::Opened;
        }

        popup->Draw();

        if (popup->_state == PopupWindow::State::Close)
        {
            popup->_state = PopupWindow::State::Closed;
            _popupQueue.pop();
        }
    }
}
