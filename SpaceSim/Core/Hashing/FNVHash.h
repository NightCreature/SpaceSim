#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/Types.h"

const size_t c_fnvHashOffset = 2166136261;
const size_t c_fnvHashPrime = 16777619;

///-----------------------------------------------------------------------------
///! @brief   Case insensitive hash function constexpr version
///! @remark  This wont cope with UTF encoded strings
///-----------------------------------------------------------------------------
inline constexpr size_t fnvStringHash(const char* strData, size_t size)
{
    size_t returnHash = c_fnvHashOffset;
    for (size_t counter = 0; counter < size; ++counter)
    {
        returnHash = returnHash ^ toLowerConstExpr(strData[counter]);
        returnHash = returnHash * c_fnvHashPrime;
    }

    return returnHash;
}

inline constexpr size_t hashBinaryData(const byte* data, size_t size)
{
    size_t returnHash = c_fnvHashOffset;
    for (size_t counter = 0; counter < size; ++counter)
    {
        returnHash = returnHash ^ data[counter];
        returnHash = returnHash * c_fnvHashPrime;
    }

    return returnHash;
}