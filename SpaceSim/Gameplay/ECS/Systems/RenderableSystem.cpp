#include "Gameplay/ECS/Systems/RenderableSystem.h"

#include "Core/Resource/Resourceable.h"
#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/MessageQueue.h"
#include "Core/Profiler/ProfilerMacros.h"

#include "Gameplay/ECS/Components/Components.h"
#include "Gameplay/ECS/Components/MaterialComponent.h"
#include "Gameplay/ECS/Entity.h"



namespace ECS
{

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void RenderableSystem::AddEntity(Entity& entity)
{
    PROFILE_FUNCTION();

    UNUSEDPARAM(entity);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void RenderableSystem::Update()
{
    PROFILE_FUNCTION();

    auto entities = m_entityManager->GetEntitiesForTag(RenderObjectComponent::GetTag() | TransformationComponent::GetTag());
    for (Entity* entity : entities)
    {
        auto* renderComponentData = entity->GetComponentData<RenderObjectComponent>();

        auto messageData = m_messageData.find(entity->GetId());
        if (messageData != m_messageData.end())
        {
            renderComponentData->m_renderObjectHandle = messageData->second;
        }

        if (renderComponentData->m_renderObjectHandle != 0)
        {
            TransformationComponent* transform = entity->GetComponentData<TransformationComponent>();
            //Have a render component here
            MessageSystem::RenderInformation renderInfo;
            MessageSystem::RenderInformation::RenderInfo data;
            data.m_renderObjectid = renderComponentData->m_renderObjectHandle;
            data.m_gameobjectid = entity->GetId();
            data.m_world = transform->m_transform;
            //data.m_name = m_name.c_str();
            renderInfo.SetData(data);
            m_resource->m_messageQueues->getUpdateMessageQueue()->addMessage(renderInfo);
        }
        else
        {
            //figure out which object we need to create and send this message to the renderer here, this is not always a face
            //This would mean that the render component carries inforamtion about what resource/asset to use
            
            //This is probably what the render component should carry for the resource request
            struct CreationParams
            {
                CommandList* m_commandList = nullptr;
                Material m_material;
                std::string m_name;
                std::string m_resourceFileName; //This would allow us to move all the specific into a resource loader, just shifting the problem but it would make the render component not have to deal with it and makes it the job of the resource loader
            };

            //This should send the message to create a render object
            //Face::CreationParams params;
            ////    params.resource = m_resource;
            ////    params.shaderInstance = &shaderInstance;
            //params.nrVerticesInX = m_rows;
            //params.nrVerticesInY = m_coloms;
            //params.width = m_widthendpos - m_widthstartpos;
            //params.height = m_heightendpos - m_heightstartpos;
            //params.fillx = m_fillx;
            //params.filly = m_filly;
            //params.fillz = m_fillz;
            //params.fillvalue = m_fillvalue;
            //params.invertNormal = m_invertNormal;
            //params.changeWindingOrder = m_changeWindingOrder;
            //params.m_materialParameters = m_materialParameters;
            //params.m_name = m_name;
            ////if (10 != m_rows && 10 != m_coloms)
            ////{
            ////    params.tesselate = false;
            ////}
            ////else
            //{
            //    params.tesselate = true;
            //}

            //auto resource = GameResourceHelper(m_resource).getWriteableResource();
            //DECLAREANDCREATERESOURCEMESSAGE(createPlaneModel, Face::CreationParams);
            //createPlaneModel.SetData(params);
            //createPlaneModel.SetGameObjectId(static_cast<size_t>(m_nameHash)); //Not super but should work for now

            //resource.m_messageQueues->getUpdateMessageQueue()->addMessage(createPlaneModel);

            //Super::initialise(shaderInstance);
        }
    }
}

///-----------------------------------------------------------------------------
///! @brief   Store the data that a render object is created and in update, update the entities with the resource ID.
///! @remark
///-----------------------------------------------------------------------------
void RenderableSystem::HandleMessage(const MessageSystem::Message& message)
{
    PROFILE_FUNCTION();
    if (MESSAGE_ID(CreatedRenderResourceMessage) == message.getMessageId())
    {
        auto& msg = static_cast<const MessageSystem::CreatedRenderResourceMessage&>(message);
        m_messageData[msg.GetData()->m_gameObjectId] = msg.GetData()->m_renderResourceHandle;
    }
}

}
