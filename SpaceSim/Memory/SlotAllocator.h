#pragma once

#include "Allocator.h"

namespace Allocator
{

template<size_t blockSize>
class SlotAllocator : public AllocatorInterface
{
public:
    SlotAllocator(size_t allocatorPoolSize) : AllocatorInterface(allocatorPoolSize), m_numberOfSlots(allocatorPoolSize/blockSize)
    {
        void* currentPos = m_start;
        for (size_t counter = 0; counter < m_numberOfSlots; ++counter)
        {
            m_freeList.push_back(currentPos);
            currentPos = (void*)((char*)currentPos + blockSize);
        }
    }
    virtual void* allocate(size_t size, unsigned char alignment = 4) override
    {

        if (m_freeList.empty())
        {
            return nullptr;
        }

        void* returnSlot = m_freeList.back();
        m_freeList.pop_back();
        m_usedMemory += blockSize;
        ++m_numAllocations;
        return returnSlot;
        //    ASSERT(size == _objectSize && alignment == _objectAlignment);
        //
        //    if (_free_list == nullptr)
        //        return nullptr;
        //
        //    void* p = _free_list;
        //
        //    _free_list = (void**)(*_free_list);
        //
        //    _used_memory += size;
        //    _num_allocations++;
        //
        //    return p;
    }

    virtual void deallocate(void* p) override
    {
        m_freeList.push_back(p);
        --m_numAllocations;
        m_usedMemory -= blockSize;
    }

//    ASSERT(objectSize >= sizeof(void*));
//
//    //Calculate adjustment needed to keep object correctly aligned
//    u8 adjustment = pointer_math::alignForwardAdjustment(mem, objectAlignment);
//
//    _free_list = (void**)pointer_math::add(mem, adjustment); //adds adjustment to the pointer value, adds second argument to first argument and returns that as a pointer.
//
//    size_t numObjects = (size - adjustment) / objectSize;
//
//    void** p = _free_list; //This is a list of pointer addresses nothing more, we store a pointer to the next free block so each slot is a pointer to another block
//    //It stores a reference to the next block of 16 bytes in the first 4 bytes, so when we overwrite a slot we can fix it up easily
//
//    //Initialize free blocks list
//    for (size_t i = 0; i < numObjects - 1; i++)
//    {
//        *p = pointer_math::add(p, objectSize);
//        p = (void**)*p;
//    }
//
//    *p = nullptr;
//}
//
//PoolAllocator::~PoolAllocator()
//{
//    _free_list = nullptr;
//}
//
//void* PoolAllocator::allocate(size_t size, u8 alignment)
//{
//    ASSERT(size == _objectSize && alignment == _objectAlignment);
//
//    if (_free_list == nullptr)
//        return nullptr;
//
//    void* p = _free_list;
//
//    _free_list = (void**)(*_free_list);
//
//    _used_memory += size;
//    _num_allocations++;
//
//    return p;
//}
//
//void PoolAllocator::deallocate(void* p)
//{
//    *((void**)p) = _free_list;
//
//    _free_list = (void**)p;
//
//    _used_memory -= _objectSize;
//    _num_allocations--;
//}
private:
    std::vector<void*> m_freeList;
    size_t m_numberOfSlots;
};

typedef SlotAllocator<sizeof(int)> IntPoolAllocator;

};