#pragma  once
#include <algorithm>
#include "vector3.h"
#include "Core/Serialization/Archive.h" 
#include <xtr1common>
#include "Core/Types/TypeHelpers.h"
#include "Core/tinyxml2.h"
#include "vector4.h"

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

    inline Vector4 Deserialise(const tinyxml2::XMLElement* node)
    {
        float x, y, z, w;
        const tinyxml2::XMLAttribute* attribute = node->FindAttribute("x");
        if (attribute)
        {
            x = attribute->FloatValue();
        }
        attribute = node->FindAttribute("y");
        if (attribute)
        {
            y = attribute->FloatValue();
        }
        attribute = node->FindAttribute("z");
        if (attribute)
        {
            z = attribute->FloatValue();
        }
        attribute = node->FindAttribute("w");
        if (attribute)
        {
            z = attribute->FloatValue();
        }

        return Vector4(x, y, z, w);
    }
}