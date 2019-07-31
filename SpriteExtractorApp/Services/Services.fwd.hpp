#pragma once

#include <memory>

template<typename T>
using ServicePtr = std::shared_ptr<T>;
