#pragma once

#include "Allocator.h"

namespace Allocator
{

template<size_t blockSize>
class SlotAllocator : public AllocatorInterface
{
public:
    SlotAllocator(size_t allocatorPoolSize) : AllocatorInterface(allocatorPoolSize)
    {
    }
    virtual void* allocate(size_t size, unsigned char alignment = 4) override
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

    virtual void deallocate(void* p) override
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

    ASSERT(objectSize >= sizeof(void*));

    //Calculate adjustment needed to keep object correctly aligned
    u8 adjustment = pointer_math::alignForwardAdjustment(mem, objectAlignment);

    _free_list = (void**)pointer_math::add(mem, adjustment); //adds adjustment to the pointer value, adds second argument to first argument and returns that as a pointer.

    size_t numObjects = (size - adjustment) / objectSize;

    void** p = _free_list; //This is a list of pointer addresses nothing more, we store a pointer to the next free block so each slot is a pointer to another block
    //It stores a reference to the next block of 16 bytes in the first 4 bytes, so when we overwrite a slot we can fix it up easily

    //Initialize free blocks list
    for (size_t i = 0; i < numObjects - 1; i++)
    {
        *p = pointer_math::add(p, objectSize);
        p = (void**)*p;
    }

    *p = nullptr;
}

PoolAllocator::~PoolAllocator()
{
    _free_list = nullptr;
}

void* PoolAllocator::allocate(size_t size, u8 alignment)
{
    ASSERT(size == _objectSize && alignment == _objectAlignment);

    if (_free_list == nullptr)
        return nullptr;

    void* p = _free_list;

    _free_list = (void**)(*_free_list);

    _used_memory += size;
    _num_allocations++;

    return p;
}

void PoolAllocator::deallocate(void* p)
{
    *((void**)p) = _free_list;

    _free_list = (void**)p;

    _used_memory -= _objectSize;
    _num_allocations--;
}
private:
};

typedef SlotAllocator<32> IntPoolAllocator;

};