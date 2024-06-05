#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"

#include <bitset>

namespace ECS
{

constexpr inline size_t NumberOfSystems = 128;
using SystemTag = std::bitset<NumberOfSystems>;
struct System;
typedef System* (*CreateSystemFP)();

inline SystemTag CreateUniqueTag()
{
    static SystemTag uniqueTag = SystemTag(1);
    SystemTag retVal = uniqueTag;
    uniqueTag <<= 1; //Shift the set bit by one
    return retVal;
}

class SystemType
{
public:
    SystemType() {}
    virtual ~SystemType() {}

    virtual const char* GetId() const = 0;
    virtual const SystemTag& GetSystemType() const = 0;
    virtual size_t GetSize() const = 0;

    virtual CreateSystemFP GetCreateFP() const = 0;
protected:
};

template<class System>
class DefaultSystemType : public SystemType
{
public:
    DefaultSystemType(const char* id, const SystemTag& tag) : m_id(id), m_tag(tag) 
    {
        m_instance = this;
    }

    const char* GetId() const override { return m_id; }
    const SystemTag& GetSystemType() const override { return m_tag; }
    size_t GetSize() const override { return sizeof(System); }
    CreateSystemFP GetCreateFP() const override { return &System::Create; }

    static DefaultSystemType* m_instance;
private:
    const char* m_id;
    SystemTag m_tag;
};

template<class System>
DefaultSystemType<System>* DefaultSystemType<System>::m_instance;

}