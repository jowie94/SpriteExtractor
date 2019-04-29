#pragma once

#include "Sprite.hpp"
#include "ModelActions.hpp"

class SpriteSheet
{
public:
	class UpdateSpritesCommand : public EditModel<SpriteSheet, std::vector<Sprite>>
	{
	public:
		UpdateSpritesCommand(const std::vector<Sprite>& newValue);
	};

	SpriteSheet();

    const std::vector<Sprite>& GetSprites() const;

private:
    std::vector<Sprite> _sprites;
};
