#include "Services.hpp"

#include "Logger/Logger.hpp"

namespace ServicesConst
{
    constexpr const std::hash<std::string_view> kServiceHash{};
}

Services& Services::GetInstance()
{
    static Services services;

    return services;
}

void Services::InitServices()
{
    Logger::LoggerPtr logger = Logger::GetLogger("Services");
    logger->info("Initializing services");

    for (const auto& service : _servicesToInit)
    {
        service();
    }

    logger->info("Initialized {} of {} services", _servicesToInit.size(), _services.size());
    _servicesToInit.clear();
}

void Services::ShutdownServices()
{
    Logger::LoggerPtr logger = Logger::GetLogger("Services");
    logger->info("Shutting down services");

    for (const auto& service : _servicesToShutdown)
    {
        service();
    }

    logger->info("Stopped {} of {} services", _servicesToShutdown.size(), _services.size());

    _servicesToShutdown.clear();
    _services.clear();
}

void Services::AddService(const std::string_view name, MapPtr ptr)
{
    _services.emplace(ServicesConst::kServiceHash(name), std::move(ptr));
}

Services::MapPtr Services::FindService(const std::string_view name) const
{
    auto it = _services.find(ServicesConst::kServiceHash(name));

    if (it != _services.end())
    {
        return it->second;
    }

    return nullptr;
}
