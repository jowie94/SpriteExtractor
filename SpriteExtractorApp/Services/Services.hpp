#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

#include "Services.fwd.hpp"

class Services
{
public:
    static Services& GetInstance();

    template<typename T, typename... Interfaces>
    void RegisterService()
    {
        ServicePtr<T> service = std::make_shared<T>();
        RegisterService<0, T, Interfaces...>(service);

        RegisterInit(service);
    }

    template<typename T>
    ServicePtr<T> Get()
    {
        return std::static_pointer_cast<T>(FindService(typeid(T).name()));
    }

    void InitServices();

private:
    using MapPtr = ServicePtr<void>;

    template<typename T>
    typename std::enable_if<!Traits::service_has_init_v<T>, void>::type RegisterInit(ServicePtr<T> service)
    {
    }

    template<typename T>
    typename std::enable_if<Traits::service_has_init_v<T>, void>::type RegisterInit(ServicePtr<T> service)
    {
        _servicesToInit.emplace_back([service] { service->Init(); });
    }

    template<int N, typename T, typename... Interfaces>
    void RegisterService(ServicePtr<void> service)
    {
        AddService(typeid(T).name(), service);
        RegisterService<0, Interfaces...>(service);
    }

    template<int N>
    void RegisterService(ServicePtr<void>)
    {
    }

    void AddService(const std::string& name, MapPtr ptr);
    MapPtr FindService(const std::string& name);

    std::unordered_map<std::string, MapPtr> _services;
    std::vector<std::function<void()>> _servicesToInit;
};

#define REGISTER_SERVICE(Service) \
    ServiceRegisterer<Service> __g_##Service

#define REGISTER_SERVICE_INTERFACE(Service, ...) \
    ServiceRegisterer<Service, __VA_ARGS__> __g_##Service

template<typename T, typename... Interfaces>
class ServiceRegisterer
{
public:
    ServiceRegisterer()
    {
        Services::GetInstance().RegisterService<T, Interfaces...>();
    }
};
