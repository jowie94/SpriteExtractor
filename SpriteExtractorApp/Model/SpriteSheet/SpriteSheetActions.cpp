#include "SpriteSheetActions.hpp"

#include <string>

#include "Sprite.hpp"
#include "SpriteSheet.hpp"

Commands::Model::UpdateSpritesCommand::UpdateSpritesCommand(const std::vector<std::shared_ptr<Sprite>>& newValue)
: EditModel(&SpriteSheet::_sprites, newValue)
{
}

void Commands::Model::UpdateSpritesCommand::redo()
{
    EditModel::redo();

    GetModel()->_selectedSpriteIdx = -1;
}

void Commands::Model::UpdateSpritesCommand::undo()
{
    EditModel::undo();

    GetModel()->_selectedSpriteIdx = -1;
}

Commands::Model::UpdateAlphaColorCommand::UpdateAlphaColorCommand(const Color& newValue)
: EditModel(&SpriteSheet::_alphaColor, newValue)
{
}

Commands::Model::UpdateSelectedSpriteCommand::UpdateSelectedSpriteCommand(int spriteIdx)
: EditModel(&SpriteSheet::_selectedSpriteIdx, spriteIdx)
{

}

Commands::Model::EditSpriteName::EditSpriteName(const std::string& spriteName, const std::string& newSpriteName)
: EditMapModel(&Sprite::Name, newSpriteName, spriteName)
{
}

SpriteSheet::SpriteSheet(std::shared_ptr<IImage> image, const std::string& imageName)
    : _image(std::move(image))
    , _imageName(imageName)
{
}
