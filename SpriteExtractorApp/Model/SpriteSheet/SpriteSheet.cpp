#include "SpriteSheet.hpp"

Commands::Model::UpdateSpritesCommand::UpdateSpritesCommand(const std::vector<std::shared_ptr<Sprite>>& newValue)
: EditModel(&SpriteSheet::_sprites, newValue)
{
}

void Commands::Model::UpdateSpritesCommand::redo()
{
    EditModel::redo();

    _model->_selectedSpriteIdx = -1;
}

void Commands::Model::UpdateSpritesCommand::undo()
{
    EditModel:undo();

    _model->_selectedSpriteIdx = -1;
}

Commands::Model::UpdateAlphaColorCommand::UpdateAlphaColorCommand(const Color& newValue)
: EditModel(&SpriteSheet::_alphaColor, newValue)
{
}

Commands::Model::UpdateSelectedSpriteCommand::UpdateSelectedSpriteCommand(int spriteIdx)
: EditModel(&SpriteSheet::_selectedSpriteIdx, spriteIdx)
{

}

SpriteSheet::SpriteSheet(std::shared_ptr<IImage> image, const std::string& imageName)
: _image(std::move(image))
, _imageName(imageName)
{
}

std::weak_ptr<const IImage> SpriteSheet::GetImage() const
{
	return _image;
}

const std::string& SpriteSheet::GetImageName() const
{
	return _imageName;
}

std::weak_ptr<const Sprite> SpriteSheet::GetSelectedSprite() const
{
	return _selectedSpriteIdx >= 0 ? _sprites.at(_selectedSpriteIdx) : nullptr;
}

const std::vector<std::shared_ptr<Sprite>>& SpriteSheet::GetSprites() const
{
	return _sprites;
}

const Color& SpriteSheet::GetAlphaColor() const
{
	return _alphaColor;
}
