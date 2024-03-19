#ifndef MODELMANGER_H
#define MODELMANGER_H

#include "Core/MessageSystem/Messages.h"
#include "Graphics/Model.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <unordered_map>

class Frustum;
class Model;
class Resource;
class ShaderInstance;
class Job;

struct ModelResourceHandle
{
    CreatedModel m_model;
    size_t m_resourceId;
};

const size_t InvalidResourceHandle = static_cast<size_t>(-1);

class ModelManager 
{
public:
    ModelManager() {}
    ~ModelManager();

    void cleanup();
    void initialise(Resource* resource) { m_resource = resource; }

    void OnMessage(const MessageSystem::Message& msg);

    size_t LoadModel(const void* data, CommandList& commandList);
    size_t AddFace(const void* data, CommandList& commandList, Job* currentJob);

    bool HasRenderResource(size_t resource_id) const;
    void RegisterCreatedModel(CreatedModel model, size_t renderResourceId);
    const CreatedModel* GetRenderResource(size_t renderResourceId) const;

    const std::vector<ModelResourceHandle>& GetModels() const
    {
        return m_models;
    }

    const std::vector<RenderInterface*> GetRenderables(const Frustum& viewFrustum) const;

    void UpdateDebugModels(size_t objectId, Matrix44 m_world) const;
protected:
private:
    bool InternalHasRenderResource(size_t resourceId) const;

    typedef std::vector<ModelResourceHandle> ModelsArray;
    std::vector<ModelResourceHandle> m_models;

    std::unordered_map<size_t, DebugRenderInterface*> m_debugBoundingBoxes;
    //std::vector<RenderInterface*> m_debugBoundingBoxes;

    Resource* m_resource;

    mutable std::mutex m_mutex;
    bool m_createDebugBoundingBoxes = true;
};
#endif