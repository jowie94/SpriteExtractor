#pragma once

#include <unordered_map>

#include "Sprite.hpp"
#include "Animation.hpp"

namespace Commands
{
    namespace Model
	{
        class UpdateSelectedSpriteCommand;
        class UpdateAlphaColorCommand;
        class UpdateSpritesCommand;
        class CreateAnimationCommand;
        class EditSpriteName;
    }
}

class SpriteSheet
{
public:
    SpriteSheet(std::shared_ptr<IImage> image, const std::string& imageName);

    std::weak_ptr<const IImage> GetImage() const;
    const std::string& GetImageName() const;

    std::weak_ptr<const Sprite> GetSelectedSprite() const;

    const std::vector<std::shared_ptr<Sprite>>& GetSprites() const;
    const Color& GetAlphaColor() const;

    bool HasSprite(const std::string& spriteName) const;

    bool HasAnimation(const std::string& animationName) const;
    std::shared_ptr<const Animation> GetAnimation(const std::string_view& animationName) const;

    std::shared_ptr<Sprite> operator[](const std::string& spriteName);
    std::shared_ptr<const Sprite> operator[](const std::string& spriteName) const;

private:
    friend class Commands::Model::UpdateSpritesCommand;
    friend class Commands::Model::UpdateAlphaColorCommand;
    friend class Commands::Model::UpdateSelectedSpriteCommand;
    friend class Commands::Model::EditSpriteName;
    friend class Commands::Model::CreateAnimationCommand;

    void GenerateAnimationsCache();

    std::shared_ptr<IImage> _image;
    std::string _imageName;

    int _selectedSpriteIdx = -1;

    std::vector<std::shared_ptr<Sprite>> _sprites;
    std::unordered_map<std::string_view, std::shared_ptr<Animation>> _animations;
    Color _alphaColor;
};
