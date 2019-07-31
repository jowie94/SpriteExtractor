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
    template<typename T>
    struct ChangeState;

    void SetupSpriteSheet();

    void DrawSprite(const BBox& spriteRect);

    std::shared_ptr<const SpriteSheet> _spriteSheet;
    std::unique_ptr<ITextureResource> _texture;

    std::unique_ptr<ChangeState<std::string>> _spriteName;
    bool _validationError = false;

    MessageBroker::SubscriptionId _openImageSubscription = -1;
};
