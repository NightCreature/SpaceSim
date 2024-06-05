#pragma once

#include <bitset>
#include "Components/ComponentTypeRegistry.h"

namespace ECS
{

struct Component
{
    virtual ~Component() {}
    //virtual const ComponentTag& GetTag() = 0;
    //virtual const ComponentType* GetType() = 0;
};

using ComponentDataCreationFp = Component * (*)();

}

#define DECLARE_COMPONENT(ComponentName) \
static const ComponentTag& GetTag() { return GetComponentTagForComponent(#ComponentName); }\
static const ComponentType* GetType() { return GetComponentTypeForComponent(#ComponentName); }\
static Component* Create() { return new ComponentName(); }
