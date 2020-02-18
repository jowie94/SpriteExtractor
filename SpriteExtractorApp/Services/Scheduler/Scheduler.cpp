#include "Scheduler.hpp"

#include <chrono>

#include "Services/Services.hpp"

REGISTER_SERVICE(Scheduler);

class Scheduler::Chrono
{
public:
    float Stop()
    {
        auto currentTs = chrono_t::now();
        std::chrono::duration<float> duration = currentTs - LastTs;
        LastTs = currentTs;
        return duration.count();
    }

private:
    using chrono_t = std::chrono::steady_clock;	
    chrono_t::time_point LastTs;
};

Scheduler::Scheduler()
: _chrono(std::make_unique<Chrono>())
{
}

void Scheduler::Schedule(void* ptr, const Callback& callback)
{
    _schedules.emplace(ptr, callback);
}

void Scheduler::Unschedule(void* ptr)
{
    _schedules.erase(ptr);
}

void Scheduler::Update()
{
    _dt = _chrono->Stop();
    for (const auto& callback : _schedules)
    {
        callback.second();
    }
}

float Scheduler::DeltaTime() const
{
    return _dt;
}
