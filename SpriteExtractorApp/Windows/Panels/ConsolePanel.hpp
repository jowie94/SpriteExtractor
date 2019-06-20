#pragma once

#include <UI/PanelWindow.hpp>

class ConsolePanel : public PanelWindow
{
public:
    ConsolePanel();

private:
    void Draw() override;

    ImFont* _font = nullptr;
    size_t _lastMessageCount = 0;
};
