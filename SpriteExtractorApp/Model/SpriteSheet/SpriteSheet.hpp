#pragma once

#include "SpriteSheet.fwd.hpp"

#include "Sprite.hpp"

class SpriteSheet
{
public:
	SpriteSheet();

    const std::vector<Sprite>& GetSprites() const;
	const Color& GetAlphaColor() const;

private:
	friend class Commands::Model::UpdateSpritesCommand;
	friend class Commands::Model::UpdateAlphaColorCommand;

    std::vector<Sprite> _sprites;
	Color _alphaColor;
};
