#pragma  once
#include <algorithm>
#include "vector3.h"
#include "Core/Serialization/Archive.h" 
#include <xtr1common>
#include "Core/Types/TypeHelpers.h"

namespace math
{
    template<class T>
    const T& clamp(const T& val, const T& min, const T& max)
    {
        return std::max<T>(min, std::min<T>(max, val));
    }

    inline void Serialise(const Archive& archive, Vector3& vec, std::true_type& tag)
    {
        UNUSEDPARAM(tag);

        float x, y, z;
        archive.Read(x);
        archive.Read(y);
        archive.Read(z);

        vec = Vector3(x, y, z);
    }


    inline void Serialise(Archive& archive, const Vector3& vec, const std::false_type& tag)
    {
        UNUSEDPARAM(tag);
        archive.Write(vec.x());
        archive.Write(vec.y());
        archive.Write(vec.z());
    }
}