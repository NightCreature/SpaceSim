#pragma once

#include "Core/Paths.h"
#include "Core/Settings/SettingsManager.h"

class Resourceable;
class Logger;
namespace MessageSystem
{
class MessageQueues;
}

#define RESOURCE_PARAMETER_LIST Logger* logger, MessageSystem::MessageQueues* messageQueues, Paths* paths, SettingsManager* settings

//Should be rewritten as an aggregation object 
class Resource
{
public:
    Resource(RESOURCE_PARAMETER_LIST) : m_logger(logger), m_messageQueues(messageQueues), m_paths(paths), m_settingsManager(settings) {}
    virtual ~Resource() {}

    Logger* m_logger;
    MessageSystem::MessageQueues* m_messageQueues;
    Paths* m_paths;
    SettingsManager* m_settingsManager;
};

class Resourceable
{
public:
    Resourceable();
    virtual ~Resourceable();

    //void registerResource(unsigned int hash, void* resource)
    //{
    //    std::map<unsigned int, void*>::const_iterator cit = m_objects.find(hash);
    //    if (cit != m_objects.end())
    //    {
    //        m_objects.insert(std::pair<unsigned int, void*>(hash, resource));
    //    }
    //}

    //void deregisterResource(unsigned int hash)
    //{
    //    std::map<unsigned int, void*>::const_iterator cit = m_objects.find(hash);
    //    if (cit != m_objects.end())
    //    {
    //        m_objects.erase(cit);
    //    }
    //}

    //template< class T >
    //T* getResource(unsigned int hash)
    //{
    //    std::map<unsigned int, void*>::iterator it = m_objects.find(hash);
    //    if (it != m_objects.end())
    //    {
    //        return (T*)it->second;
    //    }

    //    return nullptr;
    //}

    //std::map<unsigned int, void*> m_objects; //Contains hash of Type and a pointer to the resource
};

#define RESOURCE_CLASS_BEGIN(name)\
class name : public Resource\
{\
public: \
    name(RESOURCE_PARAMETER_LIST,

#define RESOURCE_CLASS_TO_INIT_LIST(name) ) : Resource(logger, messageQueues, paths, settings),
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
    name(Resource* resource) : m_resource(resource) { PROFILE_EVENT(#name, Cyan); }\
    name(const Resource* resource) : m_resource(const_cast<Resource*>(resource)) {}\
    ~name() {}\
\
    const resource_type& getResource() const { return *(static_cast<resource_type*>(m_resource)); }\
    resource_type& getWriteableResource() const { return *(static_cast<resource_type*>(m_resource)); }\
private:\
    Resource* m_resource;\
};
