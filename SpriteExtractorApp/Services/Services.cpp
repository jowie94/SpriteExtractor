#include "Services.hpp"

#include <cassert>

Services& Services::GetInstance()
{
    static Services services;

    return services;
}

void Services::InitServices()
{
    for (const auto& service : _servicesToInit)
    {
        service();
    }

    _servicesToInit.clear();
}

void Services::AddService(const std::string& name, MapPtr ptr)
{
    _services.emplace(name, std::move(ptr));
}

Services::MapPtr Services::FindService(const std::string& name)
{
    auto it = _services.find(name);

    if (it != _services.end())
    {
        return it->second;
    }

    assert(!"Service is not registered");
    return nullptr;
}
