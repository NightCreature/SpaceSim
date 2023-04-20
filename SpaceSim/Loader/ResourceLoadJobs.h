#pragma once

#include "Core/Thread/Job.h"
#include "Core/Resource/Resourceable.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Graphics/modelmanager.h"


class ResourceLoader;

////Do actual loading that update usually does 
//PROFILE_EVENT("ResourceLoaderJob::Execute", Crimson);
//
//Job* job = nullptr;
//RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
//
//if (m_request.m_resourceType == hashString("Face::CreationParams"))
//{
//    MessageSystem::CreatedRenderResourceMessage returnMessage;
//    MessageSystem::CreatedRenderResourceMessage::CreatedRenderResource data;
//    data.m_renderResourceHandle = renderResource.getModelManager().AddFace(m_request.m_loadData);//needs to be thread safe
//    data.m_gameObjectId = m_request.m_gameObjectId;
//    returnMessage.SetData(data);
//
//    renderResource.m_messageQueues->getRenderMessageQueue()->addMessage(returnMessage);
//}
//else if (m_request.m_resourceType == hashString("LOAD_TEXTURE"))
//{
//    auto textureLoadData = static_cast<char*>(m_request.m_loadData);
//    renderResource.getTextureManager().addLoad(renderResource.getDeviceManager(), textureLoadData);//needs to be thread safe, this load should probably be a job
//}
//else if (m_request.m_resourceType == hashString("LoadModelResource"))
//{
//    MessageSystem::CreatedRenderResourceMessage returnMessage;
//    MessageSystem::CreatedRenderResourceMessage::CreatedRenderResource data;
//    data.m_renderResourceHandle = renderResource.getModelManager().LoadModel(m_request.m_loadData);//needs to be thread safe, and should be a job
//    data.m_gameObjectId = m_request.m_gameObjectId;
//    returnMessage.SetData(data);
//
//    renderResource.m_messageQueues->getRenderMessageQueue()->addMessage(returnMessage);
//}
//
////This might need to be last part of the job
//delete[] m_request.m_loadData;

struct LoadRequest
{
    size_t m_resourceType = 0;
    size_t m_gameObjectId = 0;
    void* m_loadData = nullptr;

#ifdef _DEBUG
    SourceInfo m_sourceInfo;
#endif
};

class ResourceLoadJob : public Job
{
public:
    ResourceLoadJob(ResourceLoader* loader) : m_loader(loader){}

    virtual void Finish(ThreadContext* context);

    void SendReturnMsg(Resource* resource, size_t gameObjectId, size_t resourceHandle);

protected:
    CommandList* GetCommandList();
    void ReturnCommandlist(CommandList* list);
    ResourceLoader* m_loader = nullptr;
};

class FaceJob : public ResourceLoadJob
{
public:
    FaceJob( size_t gameObjectId, void* loadData, ResourceLoader* loader) : ResourceLoadJob(loader), m_gameObjectId(gameObjectId), m_loadData(loadData) {}

    bool Execute(ThreadContext* context) override;

    void Finish(ThreadContext* context) override;

private:
    size_t m_gameObjectId = 0;
    void* m_loadData = nullptr;
    size_t m_resourceHandle = InvalidResourceHandle;
};

class LoadTextureJob : public ResourceLoadJob
{
public:
    LoadTextureJob( const std::string& fileName, ResourceLoader* loader) : ResourceLoadJob(loader), m_fileName(fileName) {}

    bool Execute(ThreadContext* context) override;
private:
    std::string m_fileName;

};

class LoadModelJob : public ResourceLoadJob
{
public:
    LoadModelJob( size_t gameObjectId, void* loadData, ResourceLoader* loader) : ResourceLoadJob(loader), m_gameObjectId(gameObjectId), m_loadData(loadData) {}

    bool Execute(ThreadContext* context) override;
private:
    size_t m_gameObjectId = 0;;
    void* m_loadData = nullptr;
    size_t m_resourceHandle = InvalidResourceHandle;
};
