#include "SpriteSheet.hpp"

#include <algorithm>

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

bool SpriteSheet::HasSprite(const std::string& spriteName) const
{
    return operator[](spriteName) != nullptr;
}

bool SpriteSheet::HasAnimation(const std::string& animationName) const
{
	return GetAnimation(animationName) != nullptr;
}

std::shared_ptr<const Animation> SpriteSheet::GetAnimation(const std::string& animationName) const
{
	auto it = std::find_if(_animations.begin(), _animations.end(), [&animationName](const std::shared_ptr<const Animation>& animation)
	{
		return animation->Name == animationName;
	});

	return it == _animations.end() ? nullptr : *it;
}

std::shared_ptr<Sprite> SpriteSheet::operator[](const std::string& spriteName)
{
    auto it = std::find_if(_sprites.begin(), _sprites.end(), [&spriteName](const std::shared_ptr<Sprite>& sprite)
    {
        return sprite->Name == spriteName;
    });

    return it == _sprites.end() ? nullptr : *it;
}

std::shared_ptr<const Sprite> SpriteSheet::operator[](const std::string& spriteName) const
{
    auto it = std::find_if(_sprites.begin(), _sprites.end(), [&spriteName](const std::shared_ptr<Sprite>& sprite)
        {
            return sprite->Name == spriteName;
        });

    return it == _sprites.end() ? nullptr : *it;
}
