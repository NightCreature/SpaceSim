#pragma once

using SerializationTag = size_t;

class SerializationType
{
public:
    SerializationType() {}
    virtual ~SerializationType() {}

    virtual const char* GetId() const = 0;
    virtual const SerializationTag& GetTag() const = 0;
    virtual size_t GetSize() const = 0;

protected:
};