#pragma once

#include "SerialisationType.h"
#include "SeriaisationTypeWrapper.h"

namespace SerialisationDetails
{
    template<class T>
    concept HasCreate = requires { T::Create; };
}

template<class T>
class DefaultSerializationType : public SerializationType
{
public:
    DefaultSerializationType(const char* id, const SerializationTag& tag) : m_id(id), m_tag(tag)
    {
        m_instance = this;
    }

    const char* GetId() const override { return m_id; }
    const SerializationTag& GetTag() const override { return m_tag; }
    size_t GetSize() const override { return sizeof(T); }

    static DefaultSerializationType* m_instance;
private:
    const char* m_id;
    SerializationTag m_tag;
};

template<class T>
DefaultSerializationType<T>* DefaultSerializationType<T>::m_instance;

template<class T>
static auto GetCreateFPForSerialisationType()
{
    if constexpr (SerialisationDetails::HasCreate<T>)
    {
        return &T::Create;
    }
    else
    {
        return &SerialisationTypeWrapper<T>::Create;
    }
}
