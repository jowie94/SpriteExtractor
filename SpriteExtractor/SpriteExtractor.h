#pragma once

#include "Types.hpp"

#include <vector>
#include <functional>
#include <atomic>

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

    class Task
    {
    public:
        using CompletedCallback = std::function<void(const SpriteList&)>;

        explicit Task(CompletedCallback completedCallback_);

        void Run(const ImageAccessor& callbacks, const Color& filterColor, const void* image);
        void Stop();

        bool IsRunning() const;
    private:
        void DoRun(const ImageAccessor& callbacks, const Color& filterColor, const void* image);

        CompletedCallback completedCallback = nullptr;

        std::atomic_bool stopped;
        std::atomic_bool isRunning;
    };
};
