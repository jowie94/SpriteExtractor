#pragma once

#include <memory>

#include <UI/PanelWindow.hpp>

#include "Model/SpriteSheet/Sprite.hpp"
#include "MessageBroker.hpp"

class SpriteSheet;

namespace GenericActions
{
    struct ImageOpened;
}

class SpriteInfoPanel : public PanelWindow
{
public:
    SpriteInfoPanel();
    ~SpriteInfoPanel();

    void Init() override;

    void OnImageOpened(const GenericActions::ImageOpened& imageOpened);

protected:
    void Draw() override;

private:
    void SetupSpriteSheet();

    void DrawSprite(const BBox& spriteRect);

    std::shared_ptr<const SpriteSheet> _spriteSheet;
    std::unique_ptr<ITextureResource> _texture;

    std::string _currentSpriteName;
    std::string _tmpSpriteName;

    MessageBroker::SubscriptionId _openImageSubscription = -1;
};
