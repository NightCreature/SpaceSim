#pragma once

#include "Core/Serialization/SerializationRegister.h"

class Archive;

class ISerializable
{
public:

    virtual void Serialize(Archive& archive) = 0;
    virtual void Serialize(Archive& archive) const = 0;

    static void* Create() { return nullptr; };

    virtual const SerializationTag& GetTag() const = 0;
};