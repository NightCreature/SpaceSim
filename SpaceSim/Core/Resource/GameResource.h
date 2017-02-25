#pragma once
#include "resourceable.h"

#include "Brofiler.h"

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

//This class should contain all the accessors for the game resource, means a resource can have everthing in a list and this class is the one objects use to interfact with the systems
class GameResourceHelper
{
public:
    GameResourceHelper(Resource* resource) : m_resource(resource) { BROFILER_CATEGORY("GameResourceHelper", Profiler::Color::Cyan); }
    GameResourceHelper(const Resource* resource) : m_resource((Resource*)resource) {}
    ~GameResourceHelper() {}

    const GameResource& getGameResource() const { return *(GameResource*)m_resource; }
    GameResource& getWritableGameResource() const { return *(GameResource*)m_resource; }
private:
    Resource* m_resource;
};
