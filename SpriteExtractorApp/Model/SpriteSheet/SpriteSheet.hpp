#pragma once

#include "Sprite.hpp"
#include "Model/ModelActions.hpp"

class SpriteSheet
{
public:
	class UpdateSpritesCommand : public Commands::Model::EditModel<SpriteSheet, std::vector<Sprite>>
	{
	public:
		UpdateSpritesCommand(const std::vector<Sprite>& newValue);
	};

	SpriteSheet();

    const std::vector<Sprite>& GetSprites() const;

private:
    std::vector<Sprite> _sprites;
};
