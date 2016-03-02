#pragma once

#include "Allocator.h"

namespace Allocator
{

template<size_t alignment>
class HeapAllocator : public AllocatorInterface
{
public:
    HeapAllocator(size_t size) : AllocatorInterface(size), m_free_blocks((FreeBlock*)m_start)
    {
        ASSERT(size > sizeof(FreeBlock), "Not enough space to store allocations");

        m_free_blocks->m_size = size;
        m_free_blocks->m_next = nullptr;
    }

    ~HeapAllocator()
    {
        m_free_blocks = nullptr;
    }

    void* allocate(size_t size) override
    {
        ASSERT(size != 0 && alignment != 0, "Trying to allocate 0 bytes or alignment is 0");

        FreeBlock* prev_free_block = nullptr;
        FreeBlock* free_block = m_free_blocks;

        while (free_block != nullptr)
        {
            //Calculate adjustment needed to keep object correctly aligned
            uint8 allocationHeaderAdjustment = PointerMath::findAdjustmentrWithSpaceForHeader(free_block, alignment, sizeof(AllocationHeader));

            size_t allocationSizeNeeded = size + allocationHeaderAdjustment;

            //If allocation doesn't fit in this FreeBlock, try the next
            if (free_block->m_size < allocationSizeNeeded)
            {
                prev_free_block = free_block;
                free_block = free_block->m_next;
                continue;
            }

            static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

            //If allocations in the remaining memory will be impossible
            if (free_block->m_size - allocationSizeNeeded <= sizeof(AllocationHeader))
            {
                //Increase allocation size instead of creating a new FreeBlock
                allocationSizeNeeded = free_block->m_size;

                if (prev_free_block != nullptr)
                    prev_free_block->m_next = free_block->m_next;
                else
                    m_free_blocks = free_block->m_next;
            }
            else
            {
                //Else create a new FreeBlock containing remaining memory
                FreeBlock* next_block = (FreeBlock*)(PointerMath::addSizeToPointer(free_block, allocationSizeNeeded));
                next_block->m_size = free_block->m_size - allocationSizeNeeded;
                next_block->m_next = free_block->m_next;

                if (prev_free_block != nullptr)
                    prev_free_block->m_next = next_block;
                else
                    m_free_blocks = next_block;
            }

            uintptr_t aligned_address = (uintptr_t)free_block + allocationHeaderAdjustment;

            AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
            header->m_size = allocationSizeNeeded;
            header->m_adjustment = allocationHeaderAdjustment;

            m_usedMemory += allocationSizeNeeded;
            m_numAllocations++;

            ASSERT(PointerMath::findAdjustmentForAligmentFromCurrentPtr((void*)aligned_address, alignment) == 0, "Allocation address is not aligned");

            return (void*)aligned_address;
        }

        //ASSERT(false && "Couldn't find free block large enough!");

        return nullptr;
    }

    void deallocate(void* p) override
    {
        ASSERT(p != nullptr, "Given pointer is a nullptr");

        AllocationHeader* header = (AllocationHeader*)PointerMath::subtractSizeToPointer(p, sizeof(AllocationHeader));

        uintptr_t   block_start = reinterpret_cast<uintptr_t>(p) - header->m_adjustment;
        size_t block_size = header->m_size;
        uintptr_t   block_end = block_start + block_size;

        FreeBlock* prev_free_block = nullptr;
        FreeBlock* free_block = m_free_blocks;

        //find place of this block in the heap free blocks
        while (free_block != nullptr)
        {
            if ((uintptr_t)free_block >= block_end)
                break;

            prev_free_block = free_block;
            free_block = free_block->m_next;
        }

        if (prev_free_block == nullptr)
        {
            prev_free_block = (FreeBlock*)block_start;
            prev_free_block->m_size = block_size;
            prev_free_block->m_next = m_free_blocks;

            m_free_blocks = prev_free_block;
        }
        else if ((uintptr_t)prev_free_block + prev_free_block->m_size == block_start)
        {
            prev_free_block->m_size += block_size;
        }
        else
        {
            FreeBlock* temp = (FreeBlock*)block_start;
            temp->m_size = block_size;
            temp->m_next = prev_free_block->m_next;
            prev_free_block->m_next = temp;

            prev_free_block = temp;
        }

        //Merge blocks if they are connected in space
        if (free_block != nullptr && (uintptr_t)free_block == block_end)
        {
            prev_free_block->m_size += free_block->m_size;
            prev_free_block->m_next = free_block->m_next;
        }

        m_numAllocations--;
        m_usedMemory -= block_size;
    }

private:

    struct AllocationHeader
    {
        size_t m_size;
        uint8 m_adjustment;
    };

    struct FreeBlock
    {
        size_t     m_size;
        FreeBlock* m_next;
    };

    HeapAllocator(const HeapAllocator&) = delete;//Prevent copies because it might cause errors
    HeapAllocator& operator=(const HeapAllocator&) = delete;

    FreeBlock* m_free_blocks;
};

typedef HeapAllocator<8> Heap8Allocator;
typedef HeapAllocator<16> Heap16Allocator;

}