#include "SpriteSheetActions.hpp"

#include <string>

#include <fmt/format.h>

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
    GetModel()->GenerateAnimationsCache();
}

void Commands::Model::UpdateSpritesCommand::undo()
{
    EditModelField::undo();

    GetModel()->_selectedSpriteIdx = -1;
    GetModel()->GenerateAnimationsCache();
}

Commands::Model::UpdateAlphaColorCommand::UpdateAlphaColorCommand(const Color& newValue)
: EditModelField(&SpriteSheet::_alphaColor, newValue)
{
}

Commands::Model::UpdateSelectedSpriteCommand::UpdateSelectedSpriteCommand(int spriteIdx)
: EditModelField(&SpriteSheet::_selectedSpriteIdx, spriteIdx)
{

}

Commands::Model::EditSpriteName::EditSpriteName(const std::string& spriteName, const std::string& newSpriteName, bool updateAnimations)
: EditMapModel(&Sprite::Name, newSpriteName, spriteName)
, _updateAnimations(updateAnimations)
{
}

void Commands::Model::EditSpriteName::redo()
{
    EditMapModel::redo();

    if (_updateAnimations)
    {
        GetContainerModel()->GenerateAnimationsCache();
    }
}

void Commands::Model::EditSpriteName::undo()
{
    EditMapModel::undo();

    if (_updateAnimations)
    {
        GetContainerModel()->GenerateAnimationsCache();
    }
}

Commands::Model::CreateAnimationCommand::CreateAnimationCommand(const std::string& animationName,
	std::vector<std::string> sprites)
: NestedCommand()
{
    int i = 0;
	for (auto& sprite : sprites)
	{
        std::string newName = fmt::format("{}_{}", animationName, i);
        AddCommand<EditSpriteName>(sprite, newName, false);
        ++i;
	}
}

void Commands::Model::CreateAnimationCommand::redo()
{
    NestedCommand::redo();

	// TODO: Correct
    ::Model::Policy::DefaultPolicy<SpriteSheet>()()->GenerateAnimationsCache();
}

void Commands::Model::CreateAnimationCommand::undo()
{
    NestedCommand::undo();

    // TODO: Correct
    ::Model::Policy::DefaultPolicy<SpriteSheet>()()->GenerateAnimationsCache();
}

SpriteSheet::SpriteSheet(std::shared_ptr<IImage> image, const std::string& imageName)
: _image(std::move(image))
, _imageName(imageName)
{
}
