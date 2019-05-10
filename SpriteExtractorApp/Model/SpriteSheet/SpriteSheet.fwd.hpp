#pragma once

#include <vector>

#include "Model/ModelActions.hpp"

#include "Types.hpp"

class SpriteSheet;
struct Sprite;

namespace Commands
{
	namespace Model
	{
		class UpdateSpritesCommand : public EditModel<SpriteSheet, std::vector<Sprite>>
		{
		public:
			UpdateSpritesCommand(const std::vector<Sprite>& newValue);
		};

		class UpdateAlphaColorCommand : public EditModel<SpriteSheet, Color>
		{
		public:
			UpdateAlphaColorCommand(const Color& newValue);
		};
	}
}
