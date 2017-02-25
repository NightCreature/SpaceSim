#pragma once

#include "Resourceable.h"

#undef FIRST_RESOURCE
#undef RESOURCE
#define FIRST_RESOURCE(x)\
    class x;
#define RESOURCE(x)\
    FIRST_RESOURCE(x)
#include "RenderResources.h"

RESOURCE_CLASS_BEGIN(RenderResource)

#undef FIRST_RESOURCE
#undef RESOURCE
#define FIRST_RESOURCE(x)\
    ResourceableParam(x, x##Param)
#define RESOURCE(x)\
    ,ResourceableParam(x, x##Param)
#include "RenderResources.h"

RESOURCE_CLASS_TO_INIT_LIST(RenderResource)

#undef FIRST_RESOURCE
#undef RESOURCE
#define FIRST_RESOURCE(x)\
    ResourceInitialiser(x, x##Param)
#define RESOURCE(x) \
    ,ResourceInitialiser(x, x##Param)
#include "RenderResources.h"

RESOURCE_CLASS_CONSTRUCTOR_BODY(RenderResource)

#undef FIRST_RESOURCE
#undef RESOURCE
#define FIRST_RESOURCE(x)\
    RESOURCE(x)
#define RESOURCE(x) \
    ResourceDefinition(x)
#include "RenderResources.h"

RESOURCE_CLASS_END(RenderResource)

#undef RESOURCE