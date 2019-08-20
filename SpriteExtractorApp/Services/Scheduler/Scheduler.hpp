#pragma once

#include <functional>
#include <unordered_map>

class Scheduler
{
public:
    using Callback = std::function<void()>;

    void Schedule(void* ptr, const Callback& callback);
    void Unschedule(void* ptr);

    void Update();

private:
    std::unordered_map<void*, Callback> _schedules;
};
