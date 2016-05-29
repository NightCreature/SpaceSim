#pragma once

#include <cstdint>

namespace PointerMath
{
    inline uintptr_t alignPointer(void* ptr, size_t alignment)
    {
        return reinterpret_cast<uintptr_t>(ptr) + (alignment - 1) & (~(alignment - 1)); //Add alignment to original address than make the address a multiple of the alignment
    }

    inline uintptr_t addSizeToPointer(void* ptr, size_t sizeToAdd)
    {
        return reinterpret_cast<uintptr_t>(ptr) + sizeToAdd;
    }

    inline uintptr_t subtractSizeToPointer(void* ptr, size_t sizeToAdd)
    {
        return reinterpret_cast<uintptr_t>(ptr) - sizeToAdd;
    }

    inline size_t findAdjustmentForAligmentFromCurrentPtr(void* ptr, size_t alignment)
    {
        //                              This bit masks out anything in the original pointer and only keeps the bits that are not aligned, if this returns 0 its already aligned
        size_t adjustment = alignment - (reinterpret_cast<uintptr_t>(ptr) & (alignment - 1)); //adjustment is how many bytes we need to add to the current pointer to make it alligned

        return adjustment == alignment ? 0 : adjustment; //If we need to add a whole alignment the pointer is already aligned
    }

    inline size_t findAdjustmentrWithSpaceForHeader(void* ptr, size_t alignment, size_t headerSize)
    {
        size_t adjustment = findAdjustmentForAligmentFromCurrentPtr(ptr, alignment);

        //We can't fit the header to make it aligned so we need to move the pointer to the next aligned spot to fit in the header
        if (adjustment < headerSize)
        {
            size_t neededSpace = headerSize - adjustment; //How much is left after adjustment is subtracted from the headersize

            adjustment += alignment * (neededSpace / alignment); //If the headersize is mutiples of the alignment add enough to fit the header

            if (neededSpace % alignment > 0)
            {
                adjustment += alignment; //We had a fractional division above, working in integers so add an additional alignment to make it all fit properly.
            }
        }
        return adjustment;
    }
}