#pragma once

#include <functional>
#include <unordered_map>

class Scheduler
{
public:
    using Callback = std::function<void()>;

    Scheduler();

    void Schedule(void* ptr, const Callback& callback);
    void Unschedule(void* ptr);

    void Update();

    float DeltaTime() const;

private:
    class Chrono;

    std::unique_ptr<Chrono> _chrono;
    std::unordered_map<void*, Callback> _schedules;
    float _dt = 0.0f;
};
