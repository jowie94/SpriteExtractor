#pragma once

#include <memory>
#include <queue>

#include "UI/Popups/PopupWidget.hpp"

class PopupsController
{
public:
    void ShowPopup(std::shared_ptr<PopupWidget> popup);

private:
    friend class IMainWindowWidget;

    void Draw();

    std::queue<std::shared_ptr<PopupWidget>> _popupQueue;
};
