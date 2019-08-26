#pragma once

#include <UI/PanelWindow.hpp>

#include "AssetTypes/ImFontAsset.hpp"

class ConsolePanel : public PanelWindow
{
public:
    ConsolePanel();

private:
    void Draw() override;

    AssetPtr<ImFontAsset> _font;
    size_t _lastMessageCount = 0;
};
