//#include "Gameplay/ECS/Systems/HandleMessageSystem.h"
//
//#include "Core/Resource/GameResource.h"
//#include "Gameplay/ECS/Entity.h"
//#include "Gameplay/ECS/Components/Components.h"
//#include "Gameplay/EntityManager.h"
//
//namespace ECS
//{
//
//
//
//    /////-------------------------------------------------------------------------
//    //// @brief 
//    /////-------------------------------------------------------------------------
//    //void Plane::handleMessage(const MessageSystem::Message& msg)
//    //{
//    //    if (msg.getMessageId() == MESSAGE_ID(CreatedRenderResourceMessage))
//    //    {
//    //        const MessageSystem::CreatedRenderResourceMessage& renderResourceMsg = static_cast<const MessageSystem::CreatedRenderResourceMessage&>(msg);
//    //        renderResourceMsg.GetData();
//    //        m_renderHandle = renderResourceMsg.GetData()->m_renderResourceHandle;
//    //        //Store the render object reference we get back and the things it can do
//
//    //        //Register the bounding box with the physics
//    //        //GameResourceHelper(m_resource).getWriteableResource().getPhysicsManager().AddColidableBbox(&(m_drawableObject->getBoundingBox()));
//
//    //        m_initialisationDone = true;
//    //    }
//    //}
//
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//void HandleMessageSystem::Update()
//{
//    GameResourceHelper helper(m_resource);
//    EntityManager entityManager;// = helper.getWriteableResource().GetEntityManager();
//    std::vector<ECS::Entity*> messageEntities = entityManager.GetEntitiesForTag(ECS::MessageComponent::GetTag() | ECS::RenderObjectComponent::GetTag());
//
//    //This is potentially where issues start we need to respond to messages directly and we might not be able
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   we might want to queue these here and only really handle them when we udpate so we can run through them
/////! @remark
/////-----------------------------------------------------------------------------
//void HandleMessageSystem::handleMessage(const MessageSystem::Message& message)
//{
//    if (MESSAGE_ID(CreatedRenderResourceMessage) == message.getMessageId())
//    {
//        //Should extract the data of the message we need here
//        
//        auto& msg = static_cast<const MessageSystem::CreatedRenderResourceMessage&>(message);
//        auto data = msg.GetData();
//    }
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//void HandleMessageSystem::AddEntity(Entity& entity)
//{
//    //Should make sure this entity has the qualifying tags
//    
//}
//
//}