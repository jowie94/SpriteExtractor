#pragma once

#include "UI/BaseWindow.hpp"

class PopupWindow : public BaseWindow
{
public:
    enum class State
    {
        Open,
        Opened,
        Close,
        Closed
    };

    PopupWindow(const char* popupName, bool isCloseable);

    void Draw() override final;

    void Close();

    virtual void DrawPopup() = 0;

protected:
    State _state = State::Closed;

    bool _isCloseable = false;

private:
    friend class PopupsController;

    void BeginWidget() override final;
    void EndWidget() override final;

    bool _isDrawing = false;
};
