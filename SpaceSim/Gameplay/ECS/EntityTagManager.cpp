#include "EntityTagManager.h"
namespace ECS
{

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
Component* EntityTagManager::CreateForTag(const ComponentTag& tag)
{
    auto iterator = std::find_if(m_componentCreationFuncs.begin(), m_componentCreationFuncs.end(), [tag](const std::pair<ComponentTag, ComponentDataCreationFp>& tagFuncPair) { return tag == tagFuncPair.first; });
    if (iterator != cend(m_componentCreationFuncs))
    {
        auto tagFpPair = *iterator;
        return tagFpPair.second();
    }
    
    return nullptr;
}

}