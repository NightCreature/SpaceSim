#pragma once

#include <type_traits>

#define UNUSEDPARAM(param) (void*)&param;

template<class T>
constexpr auto castToUnderlyingType(T value) -> typename std::underlying_type<T>::type 
{
    return static_cast<std::underlying_type<T>::type>(value);
}
