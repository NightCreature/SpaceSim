#include "LightManager.h"

#include "Core/MessageSystem/RenderMessages.h"
#include "Graphics/DebugHelperFunctions.h"

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void LightManager::addLight( const std::string& name, const Light& light )
{
    unsigned int hashedName = hashString(name);
    LightMap::const_iterator it = m_lights.find(hashedName);
    if (it == m_lights.end())
    {
        m_lights.insert( LightPair(hashedName, light) );
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const Light* LightManager::getLight( const std::string& name ) const
{
    unsigned int hashedName = hashString(name);
    LightMap::const_iterator it = m_lights.find(hashedName);
    if (it != m_lights.end())
    {
        return &(it->second);
    }

    return nullptr;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
Light* LightManager::getLight( const std::string& name )
{
    unsigned int hashedName = hashString(name);
    LightMap::iterator it = m_lights.find(hashedName);
    if (it != m_lights.end())
    {
        return &(it->second);
    }

    return nullptr;
}

///-----------------------------------------------------------------------------
///! @brief   Dispatches Create light messages
///! @remark
///-----------------------------------------------------------------------------
void LightManager::dispatchMessage(const MessageSystem::Message& message)
{
    ASSERT(message.getMessageId() == MESSAGE_ID(CreateLightMessage), "Message is not a create light message should be handled elsewhere. Message id: %d", message.getMessageId());

    const MessageSystem::CreateLightMessage& lightMessage = static_cast<const MessageSystem::CreateLightMessage&>(message);
    const MessageSystem::CreateLightMessage::LightMessageData* lightData = lightMessage.getLightData();
    addLight(lightData->m_lightName, lightData->m_light);
}
