#pragma once

#include "Allocator.h"
#include "PointerArithmetic.h"

namespace Allocator
{

template<size_t alignment, size_t blockSize>
class SlotAllocator : public AllocatorInterface
{
public:
    SlotAllocator(size_t allocatorPoolSize) : AllocatorInterface(allocatorPoolSize), m_numberOfSlots(allocatorPoolSize/blockSize)
    {
        uintptr_t alignedAddress = PointerMath::alignPointer(m_start, alignment);
        void* currentPos = reinterpret_cast<void*>(alignedAddress);
        for (size_t counter = 0; counter < m_numberOfSlots; ++counter)
        {
            m_freeList.push_back(currentPos);
            currentPos = (void*)((char*)currentPos + blockSize);
        }
    }
    virtual void* allocate(size_t size) override
    {
        if (size != blockSize)
        {
            return nullptr;
        }

        if (m_freeList.empty())
        {
            return nullptr;
        }

        void* returnSlot = m_freeList.back();
        m_freeList.pop_back();
        m_usedMemory += blockSize;
        ++m_numAllocations;
        return returnSlot;
    }

    virtual void deallocate(void* p) override
    {
        m_freeList.push_back(p);
        --m_numAllocations;
        m_usedMemory -= blockSize;
    }
private:
    std::vector<void*> m_freeList;
    size_t m_numberOfSlots;
};

typedef SlotAllocator<4, sizeof(int)> IntPoolAllocator;
typedef SlotAllocator<16, 16> MatrixPoolAllocator;
typedef SlotAllocator<16, 32> Slot32PoolAllocator;
typedef SlotAllocator<16, 64> Slot64PoolAllocator;
typedef SlotAllocator<16, 128> Slot128PoolAllocator;
typedef SlotAllocator<16, 256> Slot256PoolAllocator;

};