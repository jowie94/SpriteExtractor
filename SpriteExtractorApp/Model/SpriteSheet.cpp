#include "SpriteSheet.hpp"

SpriteSheet::UpdateSpritesCommand::UpdateSpritesCommand(const std::vector<Sprite>& newValue)
: EditModel(&SpriteSheet::_sprites, newValue)
{
}

SpriteSheet::SpriteSheet()
{
    
}

const std::vector<Sprite>& SpriteSheet::GetSprites() const
{
	return _sprites;
}
