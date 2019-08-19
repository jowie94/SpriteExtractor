#include "Scheduler.hpp"

#include "Services/Services.hpp"

REGISTER_SERVICE(Scheduler);

void Scheduler::Schedule(void* ptr, Callback callback)
{
    _schedules.emplace(ptr, callback);
}

void Scheduler::Unschedule(void* ptr)
{
    _schedules.erase(ptr);
}

void Scheduler::Update()
{
    for (const auto& callback : _schedules)
    {
        callback.second();
    }
}
