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
		class UpdateSpritesCommand : public EditModel<SpriteSheet, std::vector<std::shared_ptr<Sprite>>>
		{
		public:
			UpdateSpritesCommand(const std::vector<std::shared_ptr<Sprite>>& newValue);

		private:
            void redo() override;
            void undo() override;
		};

		class UpdateAlphaColorCommand : public EditModel<SpriteSheet, Color>
		{
		public:
			UpdateAlphaColorCommand(const Color& newValue);
		};

        class UpdateSelectedSpriteCommand : public EditModel<SpriteSheet, int>
        {
        public:
            UpdateSelectedSpriteCommand(int spriteIdx);
        };
	}
}
