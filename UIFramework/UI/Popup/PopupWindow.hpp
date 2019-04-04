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

    void Close();

protected:
    State _state = State::Closed;

    bool _isCloseable = false;

private:
    friend class PopupsController;

    bool BeginWidget() override final;
    void EndWidget(bool wasDrawn) override final;

    bool _isDrawing = false;
    bool _opened = true;
};
