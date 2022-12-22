#include "Core/Hashing/SipHash.h"
#include "../StringOperations/StringHelperFunctions.h"

static constexpr uint64 key0 = 0xa26f0d36b8f2c35bull;
static constexpr uint64 key1 = 0x492213218f7f57bfull;

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
uint64 SipHash::hashString(const char* strData, size_t size)
{
    //make string lower case
    std::vector<byte> lowerCaseData;
    
    size_t remainder = size % 8;
    lowerCaseData.resize(size + (8 - remainder));
    std::fill(lowerCaseData.begin(), lowerCaseData.end(), static_cast<char>(0));
    std::transform(strData, strData + size, lowerCaseData.begin(), [](const char c) { return toLowerConstExpr(c); });

    lowerCaseData[lowerCaseData.size() - 1] = size % 256;

    //initialise hash
    uint64 v0 = 0x736f6d6570736575ull ^ key0;
    uint64 v1 = 0x646f72616e646f6dull ^ key1;
    uint64 v2 = 0x6c7967656e657261ull ^ key0;
    uint64 v3 = 0x7465646279746573ull ^ key1;

    byte* data = &lowerCaseData[0];
    uint64 currentData = 0;
    size_t bounds = (lowerCaseData.size()) / 8;
    for (size_t index = 0; index < bounds; index += 8)
    {
        currentData = *(uint64*)data;
        data += 8;
        v3 ^= currentData;
        for (size_t counter = 0; counter < 2; ++counter)
        {
            v0 += v1;
            v1 <<= 13;
            v1 ^= v0;
            v0 <<= 32;
            v2 += v3;
            v3 <<= 16;
            v3 ^= v2;
            v2 += v1;
            v1 <<= 17;
            v1 ^= v2;
            v2 <<= 32;
            v0 += v3;
            v3 <<= 21;
            v3 ^= v0;
        }
        v0 ^= currentData;
    }

    v2 ^= 0xFF;

    for (size_t counter = 0; counter < 4; ++counter)
    {
        v0 += v1;
        v1 <<= 13;
        v1 ^= v0;
        v0 <<= 32;
        v2 += v3;
        v3 <<= 16;
        v3 ^= v2;
        v2 += v1;
        v1 <<= 17;
        v1 ^= v2;
        v2 <<= 32;
        v0 += v3;
        v3 <<= 21;
        v3 ^= v0;
    }

    return v0 ^ v1 ^ v2 ^ v3;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
uint64 SipHash::hashBinaryData(const byte* data, size_t size)
{
    //initialise hash
    uint64 v0 = 0x736f6d6570736575ull ^ key0;
    uint64 v1 = 0x646f72616e646f6dull ^ key1;
    uint64 v2 = 0x6c7967656e657261ull ^ key0;
    uint64 v3 = 0x7465646279746573ull ^ key1;

    //size_t remainder = size % 8;
    //lowerCaseData.resize(size + (8 - remainder));
    //std::fill(lowerCaseData.begin(), lowerCaseData.end(), 0);

    uint64 currentData = 0;
    size_t bounds = (size + 1) / 8;
    for (size_t index = 0; index < bounds; index += 8)
    {
        currentData = *(uint64*)data;
        data += 8;
        v3 ^= currentData;
        for (size_t counter = 0; counter < 2; ++counter)
        {
            v0 += v1;
            v1 <<= 13;
            v1 ^= v0;
            v0 <<= 32;
            v2 += v3;
            v3 <<= 16;
            v3 ^= v2;
            v2 += v1;
            v1 <<= 17;
            v1 ^= v2;
            v2 <<= 32;
            v0 += v3;
            v3 <<= 21;
            v3 ^= v0;
        }
        v0 ^= currentData;
    }

    v2 ^= 0xFF;

    for (size_t counter = 0; counter < 4; ++counter)
    {
        v0 += v1;
        v1 <<= 13;
        v1 ^= v0;
        v0 <<= 32;
        v2 += v3;
        v3 <<= 16;
        v3 ^= v2;
        v2 += v1;
        v1 <<= 17;
        v1 ^= v2;
        v2 <<= 32;
        v0 += v3;
        v3 <<= 21;
        v3 ^= v0;
    }

    return v0 ^ v1 ^ v2 ^ v3;
}
