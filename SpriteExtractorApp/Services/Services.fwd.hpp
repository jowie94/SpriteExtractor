#pragma once

#include <memory>

template<typename T>
using ServicePtr = std::shared_ptr<T>;

namespace Traits
{
    template<typename Service>
    struct service_has_init
    {
    private:
        template<typename T>
        static constexpr auto check(T*) -> typename std::is_same<decltype(std::declval<T>().Init()), void>::type;

        template<typename>
        static constexpr std::false_type check(...);

        using type = decltype(check<Service>(nullptr));

    public:
        static constexpr bool value = type::value;
    };

    template<typename Service>
    constexpr bool service_has_init_v = service_has_init<Service>::value;

    template<typename Service>
    struct service_has_shutdown
    {
    private:
        template<typename T>
        static constexpr auto check(T*) -> typename std::is_same<decltype(std::declval<T>().Shutdown()), void>::type;

        template<typename>
        static constexpr std::false_type check(...);

        using type = decltype(check<Service>(nullptr));

    public:
        static constexpr bool value = type::value;
    };

    template<typename Service>
    constexpr bool service_has_shutdown_v = service_has_shutdown<Service>::value;
}
