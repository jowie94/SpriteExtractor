#pragma once

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

    PopupWidget(const char* popupName, bool isCloseable);

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
