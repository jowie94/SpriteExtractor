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

        enum class Stage
        {
            None,
            GenerateMatrix,
            FindSprites
        };

        explicit Task(CompletedCallback completedCallback);

        void Run(const ImageAccessor& callbacks, const Color& filterColor, const void* image);
        void Stop();

        bool IsRunning() const;

        Stage GetStage() const;
        float GetProgress() const;
    private:
        void DoRun(const ImageAccessor& callbacks, const Color& filterColor, const void* image);

        CompletedCallback _completedCallback = nullptr;

        std::atomic_bool _stopped;
        std::atomic_bool _isRunning;

        std::atomic<Stage> _stage;
        std::atomic<float> _progress;
    };
};
