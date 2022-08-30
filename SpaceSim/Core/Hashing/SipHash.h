#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/Types.h"

inline constexpr uint64 initialV0 = 0x736f6d6570736575ull;
inline constexpr uint64 initialV1 = 0x646f72616e646f6dull;
inline constexpr uint64 initialV2 = 0x6c7967656e657261ull;
inline constexpr uint64 initialV3 = 0x7465646279746573ull;

class SipHash
{
public:

    uint64 hashString(const char* strData, size_t size);
    uint64 hashBinaryData(const byte* data, size_t size);


private:
    void Initialise()
    {
        //Xor the key into the initial values
        m_v0 = initialV0 ^ m_key0;
        m_v1 = initialV1 ^ m_key1;
        m_v2 = initialV2 ^ m_key0;
        m_v3 = initialV3 ^ m_key1;
    }
    void Compress(size_t numberOfRounds);
    void Finalize(size_t numberOfRounds);
    void SipRound();
    //This has internal state
    uint64 m_v0 = 0x736f6d6570736575ull;
    uint64 m_v1 = 0x646f72616e646f6dull;
    uint64 m_v2 = 0x6c7967656e657261ull;
    uint64 m_v3 = 0x7465646279746573ull;

    uint64 m_finalHash;

    //This is secret set, these are just random numbers
    const uint64 m_key0 = 0xa26f0d36b8f2c35bull;
    const uint64 m_key1 = 0x492213218f7f57bfull;

    const byte* m_data = nullptr;
    size_t m_size = 0;
};
