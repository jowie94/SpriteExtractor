#include "SpriteSheetActions.hpp"

#include <string>

#include "Sprite.hpp"
#include "SpriteSheet.hpp"

Commands::Model::UpdateSpritesCommand::UpdateSpritesCommand(const std::vector<std::shared_ptr<Sprite>>& newValue)
: EditModelField(&SpriteSheet::_sprites, newValue)
{
}

void Commands::Model::UpdateSpritesCommand::redo()
{
    EditModelField::redo();

    GetModel()->_selectedSpriteIdx = -1;
}

void Commands::Model::UpdateSpritesCommand::undo()
{
    EditModelField::undo();

    GetModel()->_selectedSpriteIdx = -1;
}

Commands::Model::UpdateAlphaColorCommand::UpdateAlphaColorCommand(const Color& newValue)
: EditModelField(&SpriteSheet::_alphaColor, newValue)
{
}

Commands::Model::UpdateSelectedSpriteCommand::UpdateSelectedSpriteCommand(int spriteIdx)
: EditModelField(&SpriteSheet::_selectedSpriteIdx, spriteIdx)
{

}

Commands::Model::EditSpriteName::EditSpriteName(const std::string& spriteName, const std::string& newSpriteName)
: EditMapModel(&Sprite::Name, newSpriteName, spriteName)
{
}

Commands::Model::CreateAnimationCommand::CreateAnimationCommand(const std::string& animationName)
: InsertElement(&SpriteSheet::_animations, std::make_shared<Animation>(Animation{animationName, {}}))
{
}

SpriteSheet::SpriteSheet(std::shared_ptr<IImage> image, const std::string& imageName)
    : _image(std::move(image))
    , _imageName(imageName)
{
}
