#pragma once

#include <algorithm>

namespace ECS
{

class DataStore;

template<class DataTag>
class DataStoreArrayIterator
{
public:
    DataStoreArrayIterator(DataStore* container) : m_container(container), m_currentIndex(0) { ValidateIndex(); }
    DataStoreArrayIterator(DataStore* container, size_t index) : m_container(container), m_currentIndex(index) { ValidateIndex(); }

    DataStoreArrayIterator& operator++() { NextIndex(); return *this; }
    DataStoreArrayIterator& operator--() { PreviousIndex(); return *this; }
    template<class T>
    T& operator*() { DataTag tag;  m_container->GetDataForTag(tag, m_currentIndex); }
private:
    bool ValidateIndex();

    void NextIndex();
    void PreviousIndex();
    DataStore* m_container = nullptr;
    size_t m_currentIndex = 0;
};


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
template<class DataTag>
bool DataStoreArrayIterator<DataTag>::ValidateIndex()
{
    const auto& freeList = m_container->GetFreeList();
    const size_t maxIndex = m_container->GetMaxIndex();

    return std::find_if(freeList.begin(), freeList.end(), m_currentIndex) != freeList.end() && m_currentIndex < maxIndex;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
template<class DataTag>
void DataStoreArrayIterator<DataTag>::NextIndex()
{
    ++m_currentIndex;
    if (!ValidateIndex())
    {
        m_currentIndex = static_cast<size_t>(-1);
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
template<class DataTag>
void DataStoreArrayIterator<DataTag>::PreviousIndex()
{
    --m_currentIndex;
    if (!ValidateIndex())
    {
        m_currentIndex = static_cast<size_t>(-1);
    }
}

}