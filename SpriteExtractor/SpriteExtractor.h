#pragma once

#include "Types.hpp"

#include <vector>
#include <functional>

namespace SpriteExtractor
{
    struct ImageAccessor
    {
        std::function<size_t(const void*)> GetWidth = nullptr;
        std::function<size_t(const void*)> GetHeight = nullptr;
        std::function<Color(size_t, size_t, const void*)> GetColor = nullptr;
    };

    Matrix<bool> GenerateMatrix(const ImageAccessor& callbacks, const Color& filterColor, const void* image);

    using SpriteList = std::vector<BBox>;
    SpriteList FindSprites(const Matrix<bool>& image);
};
