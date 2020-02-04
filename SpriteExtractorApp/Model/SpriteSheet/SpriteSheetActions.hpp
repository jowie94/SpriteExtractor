#pragma once

#include <vector>

#include "Model/ModelActions.hpp"
#include "Model/SpriteSheet/SpriteSheet.hpp"

namespace Commands
{
    namespace Model
    {
        class UpdateSpritesCommand : public EditModelField<SpriteSheet, std::vector<std::shared_ptr<Sprite>>>
        {
        public:
            UpdateSpritesCommand(const std::vector<std::shared_ptr<Sprite>>& newValue);

        private:
            void redo() override;
            void undo() override;
        };

        class UpdateAlphaColorCommand : public EditModelField<SpriteSheet, Color>
        {
        public:
            UpdateAlphaColorCommand(const Color& newValue);
        };

        class UpdateSelectedSpriteCommand : public EditModelField<SpriteSheet, int>
        {
        public:
            UpdateSelectedSpriteCommand(int spriteIdx);
        };

        class EditSpriteName : public EditMapModel<SpriteSheet, Sprite, std::string>
        {
        public:
            EditSpriteName(const std::string& spriteName, const std::string& newSpriteName, bool updateAnimations = true);

        private:
            void redo() override;
            void undo() override;

            bool _updateAnimations;
        };

        class CreateAnimationCommand : public NestedCommand
        {
        public:
            CreateAnimationCommand(const std::string& animationName, std::vector<std::string> sprites);

        private:
        	void redo() override;
        	void undo() override;
        };
    }
}
