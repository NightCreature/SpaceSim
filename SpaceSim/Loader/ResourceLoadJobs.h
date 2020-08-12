#pragma once

#include "Core/Thread/Job.h"
#include "Core/Resource/Resourceable.h"


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



class ResourceLoadJob : public Job
{
public:
    ResourceLoadJob(Resource* resource) : m_resource(resource){}

    Resource* m_resource;
};

class FaceJob : public ResourceLoadJob
{

};

class LoadTextureJob : public ResourceLoadJob
{
public:
    LoadTextureJob(Resource* resource, const std::string& fileName) : ResourceLoadJob(resource), m_fileName(fileName) {}

    void Execute(size_t threadIndex) override;

private:
    std::string m_fileName;

};

class LoadModelJob : public ResourceLoadJob
{

};
