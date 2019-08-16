#pragma once

#include <type_traits>

namespace Traits 
{
    template<typename T>
    struct always_false : std::false_type
    {
    };

    template<typename T>
    constexpr typename always_false<T>::value_type always_false_v = always_false<T>::value;
}
