#pragma once

#include "Core/Paths.h"

class Logger;
class PerformanceTimer;
class Resourceable;
class SettingsManager;

namespace MessageSystem
{
class MessageQueues;
}
namespace VFS
{
    class FileSystem;
}

class DebugImgui;

#define RESOURCE_PARAMETER_LIST Logger* logger, MessageSystem::MessageQueues* messageQueues, Paths* paths, PerformanceTimer* timer, SettingsManager* settings, VFS::FileSystem* fileSystem, DebugImgui* imgui

//Should be rewritten as an aggregation object 
class Resource
{
public:
    Resource(RESOURCE_PARAMETER_LIST) : m_logger(logger), m_messageQueues(messageQueues), m_paths(paths), m_performanceTimer(timer), m_settingsManager(settings), m_fileSystem(fileSystem), m_debugImgui(imgui) {}
    virtual ~Resource() {}

    Logger* m_logger;
    MessageSystem::MessageQueues* m_messageQueues;
    Paths* m_paths;
    PerformanceTimer* m_performanceTimer;
    SettingsManager* m_settingsManager;
    VFS::FileSystem* m_fileSystem;
    DebugImgui* m_debugImgui;
};

//class Resourceable
//{
//public:
//    Resourceable();
//    virtual ~Resourceable();
//
//};

#define RESOURCE_CLASS_BEGIN(name)\
class name : public Resource\
{\
public: \
    name(RESOURCE_PARAMETER_LIST,

#define RESOURCE_CLASS_TO_INIT_LIST(name) ) : Resource(logger, messageQueues, paths, timer, settings, fileSystem, imgui),
#define RESOURCE_CLASS_CONSTRUCTOR_BODY(name) {}
#define RESOURCE_CLASS_END(name) };

#define ResourceDefinition(ResourceObject)\
    public:\
    const ResourceObject& get##ResourceObject() const { return *m_##ResourceObject; } \
    ResourceObject& get##ResourceObject() { return *m_##ResourceObject; } \
    private:\
    ResourceObject* m_##ResourceObject;
#define ResourceableParam(ResourceObject, resourceObject) ResourceObject* resourceObject
#define ResourceInitialiser(ResourceObject, resourceObject) m_##ResourceObject( resourceObject )

#define REGISTER_RESOURCE(ResourceType, ResourceInstance)\
    getResource().registerResource(hashString(#ResourceType), (void*)ResourceInstance);

#define GET_RESOURCE(ResourceType)\
    public:\
        const ResourceType& get##ResourceType() const { return *getResource().getResource<#ResourceType>(hashString(#ResourceType)); } \
        ResourceType* get##ResourceType() const { return getResource().getResource<#ResourceType>(hashString(#ResourceType)); }
    
#define RESOURCE_HELPER(name, resource_type)\
class name\
{\
public:\
    name(Resource* resource) : m_resource(resource) {  }\
    name(const Resource* resource) : m_resource(const_cast<Resource*>(resource)) {}\
    ~name() {}\
\
    const resource_type& getResource() const { return *(static_cast<resource_type*>(m_resource)); }\
    resource_type& getWriteableResource() const { return *(static_cast<resource_type*>(m_resource)); }\
private:\
    Resource* m_resource;\
};
