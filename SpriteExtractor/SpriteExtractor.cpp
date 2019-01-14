#include "SpriteExtractor.h"

#include <algorithm>
#include <thread>

namespace SpriteExtractor
{
    bool IsPointInList(const std::vector<BBox>& boxes, int x, int y)
    {
        for (const auto& box : boxes)
        {
            if (box.ContainsPoint(x, y))
            {
                return true;
            }
        }

        return false;
    }

    BBox FindSprite(size_t initialRow, size_t initialColumn, const Matrix<bool>& img)
    {
        BBox box;
        box.X = static_cast<int>(initialColumn);
        box.Y = static_cast<int>(initialRow);

        size_t minColumn = initialColumn;
        size_t maxColumn = initialColumn;
        size_t maxRow = initialRow;

        const Matrix<bool>::MatrixSize& size = img.Size();
        for (size_t y = initialRow; y < size.second; ++y)
        {
            bool pixelFound = img.At(initialColumn, y);

            if (initialColumn > 0)
            {
                for (size_t x = initialColumn - 1; x >= (minColumn - 1); --x)
                {
                    if (img.At(x, y))
                    {
                        minColumn = std::min(minColumn, x);
                        pixelFound = true;
                    }

                    if (x == 0)
                    {
                        break;
                    }
                }
            }

            for (size_t x = initialColumn; x < size.first && x <= (maxColumn + 1); ++x)
            {
                if (img.At(x, y))
                {
                    maxColumn = std::max(maxColumn, x);
                    pixelFound = true;
                }
            }

            if (!pixelFound)
            {
                break;
            }

            maxRow = y;
        }

        box.X = static_cast<int>(minColumn);
        box.Width = static_cast<int>(maxColumn - box.X);
        box.Height = static_cast<int>(maxRow - box.Y);

        return box;
    }

    Matrix<bool> GenerateMatrixImpl(const ImageAccessor& callbacks, const Color& filterColor, const void* image, std::function<bool()> exitCallback, std::function<void(float)> progressCallback)
    {
        if (exitCallback == nullptr)
        {
            exitCallback = []() { return false; };
        }

        assert(callbacks.GetWidth);
        assert(callbacks.GetHeight);
        assert(callbacks.GetColor);

        size_t width = callbacks.GetWidth(image);
        size_t height = callbacks.GetHeight(image);

        Matrix<bool> matrix(std::make_pair(width, height));

        const float totalPercentage = static_cast<float>(width * height);

        for (size_t y = 0; y < height && !exitCallback(); ++y)
        {
            for (size_t x = 0; x < width && !exitCallback(); ++x)
            {
                matrix.At(x, y) = callbacks.GetColor(x, y, image) != filterColor;

                if (progressCallback)
                {
                    progressCallback((x + (width * y)) / totalPercentage);
                }
            }
        }

        return matrix;
    }

    SpriteList FindSpritesImpl(const Matrix<bool>& image, std::function<bool()> exitCallback, std::function<void(float)> progressCallback)
    {
        if (exitCallback == nullptr)
        {
            exitCallback = []() { return false; };
        }

        SpriteList list;

        Matrix<bool>::MatrixSize size = image.Size();

        const float totalPercentage = static_cast<float>(size.first * size.second);

        for (size_t row = 0; row < size.second && !exitCallback(); ++row)
        {
            for (size_t column = 0; column < size.first && !exitCallback(); ++column)
            {
                if (image.At(column, row) && !IsPointInList(list, static_cast<int>(column), static_cast<int>(row)))
                {
                    BBox sprite = FindSprite(row, column, image);

                    column = sprite.X + sprite.Width; // Skip to the next sprite

                    list.emplace_back(sprite);
                }

                if (progressCallback != nullptr)
                {
                    progressCallback((column + (size.first * row)) / totalPercentage);
                }
            }
        }

        return list;
    }
}

Matrix<bool> SpriteExtractor::GenerateMatrix(const ImageAccessor& callbacks, const Color& filterColor, const void* image)
{
    return GenerateMatrixImpl(callbacks, filterColor, image, nullptr, nullptr);
}

SpriteExtractor::SpriteList SpriteExtractor::FindSprites(const Matrix<bool>& image)
{
    return FindSpritesImpl(image, nullptr, nullptr);
}

SpriteExtractor::Task::Task(CompletedCallback completedCallback_)
: completedCallback(completedCallback_)
, stopped(false)
, isRunning(false)
, stage(Stage::None)
, progress(0.0f)
{
}

void SpriteExtractor::Task::Run(const ImageAccessor& callbacks, const Color& filterColor, const void* image)
{
    assert(!isRunning && "Task already running");

    if (!isRunning)
    {
        progress = 0.0f;
        std::thread(std::bind(&Task::DoRun, this, callbacks, filterColor, image)).detach();
    }
}

void SpriteExtractor::Task::Stop()
{
    stopped = true;
}

bool SpriteExtractor::Task::IsRunning() const
{
    return isRunning;
}

SpriteExtractor::Task::Stage SpriteExtractor::Task::GetStage() const
{
    return stage;
}

float SpriteExtractor::Task::GetProgress() const
{
    return progress;
}

void SpriteExtractor::Task::DoRun(const ImageAccessor& callbacks, const Color& filterColor, const void* image)
{
    stopped = false;
    isRunning = true;

    auto exitCallback = [this]() -> bool { return stopped; };
    auto progressCallback = [this](float inProgress) { progress = inProgress; };

    progress = 0.0f;
    stage = Stage::GenerateMatrix;
    Matrix<bool> imageMatrix = GenerateMatrixImpl(callbacks, filterColor, image, exitCallback, progressCallback);

    progress = 0.0f;
    stage = Stage::FindSprites;
    SpriteList sprites = FindSpritesImpl(imageMatrix, exitCallback, progressCallback);

    if (!stopped)
    {
        completedCallback(sprites);
    }

    isRunning = false;
}
