#pragma once

#include "Allocator.h"
#include "SourceInformation/SourceInfo.h"

#define SOURCE_INFO SourceInfo(__FILE__, __LINE__)
#define ALLOCATE_FROM_ARENA(arena, size) arena.allocate(size, SOURCE_INFO)
#define DEALLOCATE_FROM_ARENA(ptr, size) arena.deallocate(ptr, SOURCE_INFO)

namespace Allocator
{

class MemoryArena
{
public:
    MemoryArena(AllocatorInterface& allocator) : m_allocator(allocator) {}
    ~MemoryArena()
    {
    }

    void* allocate(size_t size, const SourceInfo& sourceInfo)
    {
        return m_allocator.allocate(size);

        UNUSEDPARAM(sourceInfo);
    }

    void deallocate(void* ptr, const SourceInfo& sourceInfo)
    {
        m_allocator.deallocate(ptr);
        UNUSEDPARAM(sourceInfo);
    }

private:
    AllocatorInterface& m_allocator;
};


}