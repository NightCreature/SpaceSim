#pragma once

class Resourceable;

//Should be rewritten as an aggregation object 
class Resource
{
public:
    Resource() {}
    virtual ~Resource() {}
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
    