#pragma once

#include "Core/Serialization/SerializationRegister.h"

class Archive;

class ISerializable
{
public:

    virtual void Serialize(Archive& archive, std::true_type) = 0; //Read Data from the archive
    virtual void Serialize(Archive& archive, std::false_type) const = 0; //Write Data to the archive

    template<typename T>
    static auto Create()
    {
        if constexpr (std::is_pointer_v<T>)
        {
            return new std::remove_pointer_t<T>();
        }
        else
        {
            return T();
        }
    }

    virtual const SerializationTag& GetTag() const = 0;
};