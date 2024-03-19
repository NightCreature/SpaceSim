#pragma once

#include "Core/Resource/Resourceable.h"
#include "Core/Thread/Job.h"
#include "Core/Types/TypeErasedStorage.h"
#include "Core/Types/Types.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Graphics/modelmanager.h"

#include <memory>
#include <source_location>



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
    LoadRequest() = default;

    template<class T>
    LoadRequest(const T& data) : m_loadData(data) {}

    template<class T>
    LoadRequest(const T* data, size_t dataSize) : m_loadData(data, dataSize) {}

    //template<class T, size_t size>
    //LoadRequest(const T* data) : m_loadData(data) {}

    // This type can only be moved or constructed not copied
    LoadRequest(LoadRequest&& other) :
        m_resourceType(other.m_resourceType),
        m_gameObjectId(other.m_gameObjectId),
        m_loadData(std::move(other.m_loadData))
    {}

    LoadRequest(LoadRequest&) = delete;

    size_t m_resourceType = 0;
    size_t m_gameObjectId = 0;
    TypeErasedStorage m_loadData;

#ifdef _DEBUG
    std::source_location m_sourceInfo;
#endif
};

class ResourceLoadJob : public Job
{
public:
    ResourceLoadJob(ResourceLoader* loader) : m_loader(loader){}
    ResourceLoadJob(LoadRequest&& loadRequest, ResourceLoader* loader) : m_loader(loader), m_request(std::move(loadRequest)) {}

    virtual void Finish(ThreadContext* context);

    void SendReturnMsg(Resource* resource, size_t gameObjectId, size_t resourceHandle);

protected:
    CommandList* GetCommandList();
    void ReturnCommandlist(CommandList* list);
    ResourceLoader* m_loader = nullptr;
    LoadRequest m_request;
};

class FaceJob : public ResourceLoadJob
{
public:
    FaceJob(LoadRequest&& request, ResourceLoader* loader) : ResourceLoadJob(std::move(request), loader) {}

    bool Execute(ThreadContext* context) override;

    void Finish(ThreadContext* context) override;

private:
    size_t m_resourceHandle = InvalidResourceHandle;
};

class LoadTextureJob : public ResourceLoadJob
{
public:
    LoadTextureJob(LoadRequest&& request, ResourceLoader* loader) : ResourceLoadJob(std::move(request), loader) {}

    bool Execute(ThreadContext* context) override;
private:
    std::string m_fileName;

};

class LoadModelJob : public ResourceLoadJob
{
public:
    LoadModelJob(LoadRequest&& request, ResourceLoader* loader) : ResourceLoadJob(std::move(request), loader) {}

    bool Execute(ThreadContext* context) override;
private:
    size_t m_resourceHandle = InvalidResourceHandle;
};

class LoadTextureListJob : public ResourceLoadJob
{
public:
    LoadTextureListJob(LoadRequest&& loadrequest, ResourceLoader* loader) : ResourceLoadJob(std::move(loadrequest), loader) {}

    bool Execute(ThreadContext* context) override;
};
