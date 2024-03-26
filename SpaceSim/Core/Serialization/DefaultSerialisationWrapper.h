#pragma once

#include "SerialisationType.h"

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



    template<class U>
    auto GetCreateFP() const
    {
        if constexpr (SerialisationDetails::HasCreate<U>)
        {
            return &U::Create;
        }
        else
        {
            return &SerialisationTypeWrapper<U>::Create;
        }
    }

    static DefaultSerializationType* m_instance;
private:
    const char* m_id;
    SerializationTag m_tag;
};

template<class T>
DefaultSerializationType<T>* DefaultSerializationType<T>::m_instance;