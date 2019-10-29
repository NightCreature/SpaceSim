#pragma once

#include "Memory/Allocator.h"
#include "Memory/PointerArithmetic.h"
#include "Core/Types/TypeHelpers.h"

namespace Allocator
{

template<size_t alignment>
class LinearAllocator : public AllocatorInterface
{
public:
    LinearAllocator(size_t size) : AllocatorInterface(size)
    {
        currentPosition = reinterpret_cast<void*>(PointerMath::alignPointer(m_start, alignment));
    }

    ///-----------------------------------------------------------------------------
    ///! @brief   TODO enter a description
    ///! @remark
    ///-----------------------------------------------------------------------------
    virtual void* allocate(size_t size) override
    {
        if (m_usedMemory + size > m_size)
        {
            return nullptr;
        }

        void* returnAddress = currentPosition;
        currentPosition = reinterpret_cast<void*>(PointerMath::alignPointer(reinterpret_cast<void*>( reinterpret_cast<uintptr_t>(currentPosition) + size ), alignment)); //align the current position for next allocation

        m_usedMemory += reinterpret_cast<uintptr_t>(currentPosition) - reinterpret_cast<uintptr_t>(returnAddress);
        ++m_numAllocations;

#ifdef MEMORY_PROFILING
        //Tell mtuner about the allocation
        rmemAlloc(0, returnAddress, size, m_usedMemory - size);
#endif

        return returnAddress;
    }

    ///-----------------------------------------------------------------------------
    ///! @brief   TODO enter a description
    ///! @remark
    ///-----------------------------------------------------------------------------
    virtual void deallocate(void* p) override
    {
        UNUSEDPARAM(p);
#ifdef MEMORY_PROFILING
        //Tell mtuner about the allocation
        rmemFree(0, p);
#endif
    }

    void clear()
    {
        m_numAllocations = 0;
        m_usedMemory = 0;

        currentPosition = m_start;
    }


private:
    void* currentPosition;
};

typedef LinearAllocator<4> Block4AlignedAllocator;
typedef LinearAllocator<8> Block8AlignedAllocator;
typedef LinearAllocator<16> Block16AlignedAllocator;

}