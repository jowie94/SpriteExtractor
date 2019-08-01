#pragma once

#include <UI/PanelWindow.hpp>

#include "AssetTypes/FontAsset.hpp"

class ConsolePanel : public PanelWindow
{
public:
    ConsolePanel();

private:
    void Draw() override;

    AssetPtr<FontAsset> _font;
    size_t _lastMessageCount = 0;
};
