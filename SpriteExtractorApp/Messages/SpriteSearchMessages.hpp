#pragma once

#include "SpriteExtractor.h"

#include "Model/Sprite.hpp"

namespace SpriteSearchMessages
{
    struct ProgressUpdate
    {
        SpriteExtractor::Task::Stage Stage = SpriteExtractor::Task::Stage::None;
        float Progress = 0.0f;
    };

    using SpriteList = std::vector<Sprite>;
    struct SpriteSearchFinished
    {
        SpriteSearchFinished(size_t spritesFound_)
		: spritesFound(spritesFound_)
        {}

		size_t spritesFound;
    };
}
