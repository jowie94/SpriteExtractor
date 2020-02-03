#include "SpriteSheet.hpp"

#include <algorithm>
#include <regex>

#include "Logger/Logger.hpp"

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

std::shared_ptr<const Animation> SpriteSheet::GetAnimation(const std::string_view& animationName) const
{
    auto it = _animations.find(animationName);

    return it == _animations.end() ? nullptr : it->second;
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

void SpriteSheet::GenerateAnimationsCache()
{
    _animations.clear();

    std::regex regex("^(.+)_(\\d+)$");
    std::cmatch match;

    std::shared_ptr<Animation> lastAnimation;

    Logger::LoggerPtr logger = Logger::GetLogger("Animations");

    for(const auto& sprite : _sprites)
    {
        if (std::regex_match(sprite->Name.c_str(), match, regex))
        {
            const auto& animationName = match[1].str();
            if (!lastAnimation || lastAnimation->Name != match[1].str())
            {
                auto animationIt = _animations.find(animationName);
                if (animationIt == _animations.end())
                {
                    logger->info("Found new animation: {}", animationName);
                    lastAnimation = std::make_shared<Animation>();
                    lastAnimation->Name = animationName;
                    _animations.emplace(animationName, lastAnimation);
                }
                else
                {
                    lastAnimation = animationIt->second;
                }
            }

            lastAnimation->SpriteIds.emplace_back(sprite->Idx);
        }
    }

    logger->info("Found {} animations", _animations.size());
}
