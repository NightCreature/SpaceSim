#include "DataStore.h"

#include "Entity.h"
#include "Core/Types/TypeHelpers.h"

namespace ECS
{

//class EntityTemplate
//{};
//
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//Entity DataStore::CreateEntity(EntityTemplate entityTemplate)
//{
//    UNUSEDPARAM(entityTemplate);
//    Entity entity;
//
//    //entity.m_data = this;
//    if (m_freeEntities.empty())
//    {
//        entity.m_entityId = m_currentFreeIndex; //Should be the current free index
//        ++m_currentFreeIndex;
//    }
//    else
//    {
//        entity.m_entityId = m_freeEntities.front();
//        m_freeEntities.pop_front();
//    }
//
//    //Need to make the data we need here
//
//    return entity;
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//void DataStore::DestroyEntity(Entity entity)
//{
//    if (entity.m_entityId + 1 == m_currentFreeIndex)
//    {
//        //Last object in the data arrays so we dont care just decrease the free index
//        --m_currentFreeIndex;
//        //We should probably check if any of the free list indices now is at the end of the array because we can return them, this is a recursive operation for the amount of ids in the list
//    }
//    else
//    {
//        //This entity is somewhere in the middle of the list
//        m_freeEntities.push_back(entity.m_entityId);
//    }
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//void DataStore::AddTag(DataTag* tag)
//{
//    auto iterator = std::find_if(m_data.begin(), m_data.end(), [tag](const auto& dataPair) { if (dataPair.first == tag) { return true; } return false; });
//    if (iterator == m_data.end())
//    { 
//        //New data storage object 
//        std::vector<void*> dataObject;
//        dataObject.resize(32 * sizeof(*tag)); //allocate space for 32 entities;
//        m_data.push_back(std::make_pair(tag, dataObject));
//    }
//}

}