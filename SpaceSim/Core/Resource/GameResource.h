#pragma once
#include "Resourceable.h"

#include "Core/Profiler/ProfilerMacros.h"

//Following macros generate a class and a predeclarion list of the reource objects that should be in the resource defined below

#undef FIRST_RESOURCE
#undef RESOURCE
#define FIRST_RESOURCE(x)\
    class x;
#define RESOURCE(x)\
    FIRST_RESOURCE(x)
#include "GameResources.h"


RESOURCE_CLASS_BEGIN(GameResource)

#undef FIRST_RESOURCE
#undef RESOURCE
#define FIRST_RESOURCE(x)\
    ResourceableParam(x, x##Param)
#define RESOURCE(x)\
    ,FIRST_RESOURCE(x)
#include "GameResources.h"

RESOURCE_CLASS_TO_INIT_LIST(GameResource)

#undef FIRST_RESOURCE
#undef RESOURCE
#define FIRST_RESOURCE(x)\
    ResourceInitialiser(x, x##Param)
#define RESOURCE(x) \
    ,FIRST_RESOURCE(x)
#include "GameResources.h"

RESOURCE_CLASS_CONSTRUCTOR_BODY(GameResource)

#undef FIRST_RESOURCE
#undef RESOURCE
#define FIRST_RESOURCE(x)\
    ResourceDefinition(x)
#define RESOURCE(x) \
    FIRST_RESOURCE(x)
#include "GameResources.h"

RESOURCE_CLASS_END(GameResource)

RESOURCE_HELPER(GameResourceHelper, GameResource)
