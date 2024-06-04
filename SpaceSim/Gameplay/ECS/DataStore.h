#pragma once

#include "DataStoreArrayIterator.h"
#include "DataTag.h"

#include <deque>
#include <vector>
#include "Core/Types/TypeHelpers.h"

namespace ECS
{
//class DataStore;
//class Entity;
//class EntityTemplate;
//
////Stores most data in arrays, entities are handles to this stuff and as such mostly an index to the arrays here
//class DataStore
//{
//public:
//    Entity CreateEntity(EntityTemplate entityTemplate);
//    void DestroyEntity(Entity entity);
//
//    size_t NumberOfEntities() { return m_currentFreeIndex - m_freeEntities.size(); }
//
//    void AddTag(DataTag* tag);
//
//    template<class T>
//    void AddData(const T& data, DataTag* tag, size_t index);
//
//    template<class T>
//    const T* GetDataForTag(DataTag* tag, size_t index);
//    template<class T>
//    const std::vector<T>* GetDataForTag(DataTag* tag);
//
//    template<class DataTag>
//    DataStoreArrayIterator<DataTag>& Begin()
//    {
//        return DataStoreArrayIterator<DataTag>(this);
//    }
//
//    template<class DataTag>
//    DataStoreArrayIterator<DataTag>& End()
//    {
//        DataStoreArrayIterator<DataTag> iterator(this, static_cast<size_t>(-1));
//        return iterator;
//    }
//
//    const std::deque<size_t>& GetFreeList() { return m_freeEntities; }
//    size_t GetMaxIndex() { return m_currentFreeIndex; }
//private:
//    std::deque<size_t> m_freeEntities;
//    size_t m_currentFreeIndex = 0;
//
//    std::vector<std::pair<DataTag*, std::vector<void*>>> m_data;
//};
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<class T>
//const std::vector<T>* DataStore::GetDataForTag(DataTag* tag)
//{
//    auto iterator = std::find_if(m_data.begin(), m_data.end(), [tag](const auto& dataPair) { if (dataPair.first == tag) { return true; } return false; });
//    if (iterator != m_data.end())
//    {
//        return &iterator; //this is bad
//    }
//    return nullptr;
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<class T>
//const T* DataStore::GetDataForTag(DataTag* tag, size_t index)
//{
//    auto iterator = std::find_if(m_data.begin(), m_data.end(), [tag](const auto& dataPair) { if (dataPair.first == tag) { return true; } return false; });
//    if (iterator != m_data.end())
//    {
//        if (iterator->second.size() > index)
//        {
//            return static_cast<T*>(iterator->second[index]);
//        }
//    }
//    return nullptr;
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<class T>
//void DataStore::AddData(const T& data, DataTag* tag, size_t index)
//{
//    UNUSEDPARAM(index);
//    UNUSEDPARAM(data);
//
//    auto iterator = std::find_if(m_data.begin(), m_data.end(), [tag](const auto& dataPair) { if (dataPair.first == tag) { return true; } return false; });
//    if (iterator != m_data.end())
//    {
//        //if (index < iterator->second.size())
//        //{
//        //    iterator->second[index] = data;
//        //}
//        //else
//        //{
//        //    iterator->second.push_back(data); //this is wrong
//        //}
//    }
//}

}