#pragma once

#include <string>

#include <imgui.h>

#include "UI/Widgets/IWidget.hpp"

class PopupWidget : public IWidget
{
public:
    enum class State
    {
        Open,
        Opened,
        Close,
        Closed
    };

    PopupWidget(const std::string& popupName, bool isCloseable);

    void Draw() override final;

    void Close();

    virtual void DrawPopup() = 0;

protected:
    State _state = State::Closed;

    std::string _popupName;
    bool _isCloseable = false;

private:
    friend class PopupsController;

    bool _isDrawing = false;
};
