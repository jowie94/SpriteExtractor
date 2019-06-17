#pragma once

#include <UI/PanelWindow.hpp>

class ConsolePanel : public PanelWindow
{
public:
    ConsolePanel();

private:
    void Draw() override;

    bool _autoScroll = true;
    ImFont* _font = nullptr;
};
