#pragma once

#include "SerialisationType.h"
#include "SeriaisationTypeWrapper.h"
#include "DefaultSerialisationWrapper.h"

#include <typeinfo>
#include <unordered_map>
#include <type_traits>


class ISerializable;

using SerializationTag = size_t;

namespace SerializationDetails
{
    inline SerializationTag CreateUniqueTag()
    {
        static SerializationTag uniqueTag = SerializationTag(0);
        SerializationTag retVal = uniqueTag;
        uniqueTag++;
        return retVal;
    }
}

struct SerialisableRegistry
{
    std::unordered_map<const char*, SerializationTag> m_tagsById;
    std::unordered_map<SerializationTag, const SerializationType*> m_typesByTag;
};

static SerialisableRegistry theSerialisableRegister;
inline SerialisableRegistry& GetSerialisableRegister() { return theSerialisableRegister; }

inline void RegisterSerializableType(const SerializationType* type)
{
    auto& theRegister = GetSerialisableRegister();

    theRegister.m_tagsById.insert({ type->GetId(), type->GetTag() });
    theRegister.m_typesByTag.insert({ type->GetTag(), type });
}

template<class SerializationClass>
SerializationTag RegisterSerializationType(const char* name, SerializationTag tag = SerializationDetails::CreateUniqueTag())
{
    static DefaultSerializationType<SerializationClass> serialisationType(name, tag);

    RegisterSerializableType(&serialisationType);

    return tag;
}

//This just inserts a name and a tag for a type, there is no special stuff here because we cant get that
template<class SerializationType>
SerializationTag RegisterNonSerialisationType(const char* name, SerializationTag tag = SerializationDetails::CreateUniqueTag())
{
    static DefaultSerializationType<SerialisationTypeWrapper<SerializationType>> serialisationType(name, tag);

    RegisterSerializableType(&serialisationType);

    return tag;
}

inline const SerializationTag& GetSerializationTagByName(const char* name)
{
    auto& registery = GetSerialisableRegister();
    return registery.m_tagsById[name];
}

inline const SerializationType* GetSerializationTypeByName(const char* name)
{
    auto& registery = GetSerialisableRegister();
    return registery.m_typesByTag[registery.m_tagsById[name]];
}

inline const SerializationType* GetSerializationTypeByTag(const SerializationTag& tag)
{
    auto& registery = GetSerialisableRegister();
    return registery.m_typesByTag[tag];
}

inline const SerializationTag* GetSerializationTagByNamePtr(const std::string_view& name)
{
    auto& registery = GetSerialisableRegister();
    if (registery.m_tagsById.contains(name.data()))
    {
        return &(registery.m_tagsById[name.data()]);
    }

    return nullptr;
}

#define REGISTER_SERIALIZATION_OBJECT(CLASS)\
static ISerializable* Create() { return new CLASS(); }\
static const SerializationTag& GetSerializationTag() { return GetSerializationTagByName(#CLASS); }\
virtual const SerializationTag& GetTag() const { return CLASS::GetSerializationTag(); }

#define IMPLEMENT_SERIALIZATION_OBJECT(CLASS)\
static SerializationTag tag##CLASS = RegisterSerializationType<CLASS>(#CLASS);

#define REGISTER_SERIALIZATION_TEMPLATE(CLASS)\
static inline ISerializable* Create() { return new CLASS(); }\
static inline const SerializationTag m_tag = RegisterSerializationType<CLASS>(typeid(CLASS()).name());\
static const SerializationTag& GetSerializationTag() { return GetSerializationTagByName(typeid(CLASS()).name()); }\
virtual const SerializationTag& GetTag() const { return CLASS::GetSerializationTag(); }