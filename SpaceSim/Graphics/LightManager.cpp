#include "LightManager.h"

#include "Core/MessageSystem/RenderMessages.h"
#include "Graphics/DebugHelperFunctions.h"
#include "Core/Resource/RenderResource.h"
#include "Core/Profiler/ProfilerMacros.h"
#include "Core/StringOperations/StringHelperFunctions.h"

constexpr size_t maxLights = 32;

struct LightConstantData
{
    LightConstants m_lights[maxLights];
    uint m_activeNumberOfLights;
};

void LightManager::Initialise(Resource* resource)
{
    PROFILE_FUNCTION();
    RenderResourceHelper renderResourceHelper(resource);
    RenderResource& renderResource = renderResourceHelper.getWriteableResource();

    m_lightBuffer.Create(renderResource.getDeviceManager(), renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap(), sizeof(LightConstantData), "LightContants");
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void LightManager::addLight( const std::string& name, const Light& light )
{
    auto hashedName = Hashing::hashString(name);
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
    auto hashedName = Hashing::hashString(name);
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
    auto hashedName = Hashing::hashString(name);
    LightMap::iterator it = m_lights.find(hashedName);
    if (it != m_lights.end())
    {
        return &(it->second);
    }

    return nullptr;
}

void LightManager::update()
{
    //copy over the data to the constant buffer and upload to GPU, this might need to select which lights should go into the buffer and resize it if necessary
    LightConstantData constantData;
    m_activeLights = 0;
    for (auto&& light : m_lights)
    {
        ASSERT(m_activeLights < maxLights, "Trying to set more lights than supported, max amount: %d", maxLights);
        constantData.m_lights[m_activeLights] = light.second.getLightConstants();
        ++m_activeLights;
    }
    constantData.m_activeNumberOfLights = static_cast<uint>(m_activeLights);
    m_lightBuffer.UpdateCpuData(constantData);
    m_lightBuffer.UpdateGpuData();
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
