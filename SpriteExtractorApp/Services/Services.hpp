#pragma once

#include <unordered_map>

#include "Services.fwd.hpp"

class Services
{
public:
    static Services& GetInstance();

    template<typename T, typename... Args>
    void RegisterService(Args&&... args)
    {
        std::shared_ptr<T> service = std::make_shared<T>(std::forward<Args>(args)...);
        AddService(typeid(T).name(), std::move(service));
    }

    template<typename T>
    ServicePtr<T> Get()
    {
        return std::static_pointer_cast<T>(FindService(typeid(T).name()));
    }

private:
    using MapPtr = ServicePtr<void>;

    void AddService(const std::string& name, MapPtr&& ptr);
    MapPtr FindService(const std::string& name);

    std::unordered_map<std::string, MapPtr> _services;
};

#define REGISTER_SERVICE(Service) \
    ServiceRegisterer<Service> __g_##Service

template<typename T>
class ServiceRegisterer
{
public:
    ServiceRegisterer()
    {
        Services::GetInstance().RegisterService<T>();
    }
};
