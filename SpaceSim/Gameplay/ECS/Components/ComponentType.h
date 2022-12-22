#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"

#include <bitset>

namespace ECS
{

constexpr inline size_t NumberOfComponents = 128;
using ComponentTag = std::bitset<NumberOfComponents>;
struct Component;
typedef Component* (*CreateComponentFP)();

inline ComponentTag CreateUniqueTag()
{
    static ComponentTag uniqueTag = ComponentTag(1);
    ComponentTag retVal = uniqueTag;
    uniqueTag <<= 1; //Shift the set bit by one
    return retVal;
}

class ComponentType
{
public:
    ComponentType() {}
    virtual ~ComponentType() {}

    virtual const char* GetId() const = 0;
    virtual const ComponentTag& GetComponentType() const = 0;
    virtual size_t GetSize() const = 0;

    virtual CreateComponentFP GetCreateFP() const = 0;
protected:
};

template<class Component>
class DefaultComponentType : public ComponentType
{
public:
    DefaultComponentType(const char* id, const ComponentTag& tag) : m_id(id), m_tag(tag) 
    {
        m_instance = this;
    }

    const char* GetId() const override { return m_id; }
    const ComponentTag& GetComponentType() const override { return m_tag; }
    size_t GetSize() const override { return sizeof(Component); }
    CreateComponentFP GetCreateFP() const override { return &Component::Create; }

    static DefaultComponentType* m_instance;
private:
    const char* m_id;
    ComponentTag m_tag;
};

template<class Component>
DefaultComponentType<Component>* DefaultComponentType<Component>::m_instance;

}