#pragma once

#include <cstdint>

namespace PointerMath
{
    inline uintptr_t alignPointer(void* ptr, size_t alignment)
    {
        return reinterpret_cast<uintptr_t>(ptr) + (alignment - 1) & (~(alignment - 1)); //Add alignment to original address than make the address a multiple of the alignment
    }
}