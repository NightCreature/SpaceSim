#pragma once

#include <typeinfo>
#include <unordered_map>

class ISerializable;

using SerializationTag = size_t;
class SerializationType
{
public:
    SerializationType() {}
    virtual ~SerializationType() {}

    virtual const char* GetId() const = 0;
    virtual const SerializationTag& GetTag() const = 0;
    virtual size_t GetSize() const = 0;

    typedef ISerializable* (*Create)();
    virtual Create GetCreateFP() const = 0;
protected:
};

namespace SerializationDetails
{
    
    inline SerializationTag CreateUniqueTag()
    {
        static SerializationTag uniqueTag = SerializationTag(0);
        SerializationTag retVal = uniqueTag;
        uniqueTag++;
        return retVal;
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
        Create GetCreateFP() const override { return &T::Create; }

        static DefaultSerializationType* m_instance;
    private:
        const char* m_id;
        SerializationTag m_tag;
    };

    template<class T>
    DefaultSerializationType<T>* DefaultSerializationType<T>::m_instance;

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
    static SerializationDetails::DefaultSerializationType<SerializationClass> serialisationType(name, tag);

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

//
//inline const ComponentType* GetComponentTypeForComponent(const char* componentName)
//{
//    auto& registery = GetComponentRegistry();
//    return registery.m_componentTypes[componentName];
//}
//
//inline Component* CreateComponent(const char* componentName)
//{
//    auto& registery = GetComponentRegistry();
//    return registery.m_idToCreationFp[componentName]();
//}

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