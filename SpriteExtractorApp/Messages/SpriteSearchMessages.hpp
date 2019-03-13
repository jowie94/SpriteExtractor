#pragma once

#include "SpriteExtractor.h"

namespace SpriteSearchMessages
{
    struct ProgressUpdate
    {
        SpriteExtractor::Task::Stage Stage = SpriteExtractor::Task::Stage::None;
        float Progress = 0.0f;
    };

    struct SpriteSearchFinished
    {
        SpriteSearchFinished(std::weak_ptr<SpriteList> foundSprites)
        : FoundSprites(foundSprites)
        {}

        std::weak_ptr<SpriteList> FoundSprites;
    };
}
