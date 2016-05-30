#pragma  once
#include <algorithm>

namespace math
{
    template<class T>
    const T& clamp(const T& val, const T& min, const T& max)
    {
        return std::max<T>(min, std::min<T>(max, val));
    }
}