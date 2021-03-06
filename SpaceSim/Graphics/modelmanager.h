#ifndef MODELMANGER_H
#define MODELMANGER_H

#include "Core/MessageSystem/Messages.h"

#include "Graphics/Model.h"

#include <string>
#include <vector>
#include <map>

class Model;
class Resource;
class ShaderInstance;

class ModelManager 
{
public:
    ModelManager() {}
    ~ModelManager();

    void cleanup();
    void initialise(Resource* resource) { m_resource = resource; }

    size_t LoadModel(void* data);
    size_t AddFace(void* data);

    bool HasRenderResource(size_t resource_id) const;
    void RegisterCreatedModel(CreatedModel model, size_t renderResourceId);
    const CreatedModel* GetRenderResource(size_t renderResourceId) const;
protected:
private:
    struct ModelResourceHandle
    {
        CreatedModel m_model;
        size_t m_resourceId;        
    };
    typedef std::vector<ModelResourceHandle> ModelsArray;
    std::vector<ModelResourceHandle> m_models;

    Resource* m_resource;
};
#endif