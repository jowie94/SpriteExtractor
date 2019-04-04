#pragma once

#include <memory>
#include <queue>

class PopupWindow;

class PopupsController
{
public:
    void ShowPopup(std::shared_ptr<PopupWindow> popup);

private:
    friend class BaseMainWindow;

    void Draw();

    std::queue<std::shared_ptr<PopupWindow>> _popupQueue;
};
