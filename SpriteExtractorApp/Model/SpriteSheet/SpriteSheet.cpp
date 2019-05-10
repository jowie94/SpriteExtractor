#include "SpriteSheet.hpp"

Commands::Model::UpdateSpritesCommand::UpdateSpritesCommand(const std::vector<Sprite>& newValue)
: EditModel(&SpriteSheet::_sprites, newValue)
{
}

Commands::Model::UpdateAlphaColorCommand::UpdateAlphaColorCommand(const Color& newValue)
: EditModel(&SpriteSheet::_alphaColor, newValue)
{
}

SpriteSheet::SpriteSheet()
{
    
}

const std::vector<Sprite>& SpriteSheet::GetSprites() const
{
	return _sprites;
}

const Color& SpriteSheet::GetAlphaColor() const
{
	return _alphaColor;
}
