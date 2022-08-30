#include "Core/Hashing/SipHash.h"
#include "../StringOperations/StringHelperFunctions.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
uint64 SipHash::hashString(const char* strData, size_t size)
{
    std::vector<byte> data;
    std::transform(strData, strData + size, data.begin(), [](const char c) { return toLowerConstExpr(c); });
    m_data = &data[0];
    m_size = size;

    Initialise();
    Compress(2);
    Finalize(4);

    return m_finalHash;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
uint64 SipHash::hashBinaryData(const byte* data, size_t size)
{
    m_data = data;
    m_size = size;

    Initialise();
    Compress(2);
    Finalize(4);

    return m_finalHash;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SipHash::Compress(size_t numberOfRounds)
{
    //Need to make sure the size is 8 bytes in length

    //Select the uint64 representing the 8 byte data block for the current index
    const byte* data = m_data;
    uint64 currentData = 0;
    size_t bounds = (m_size + 1) / 8;
    for (size_t index = 0; index < bounds; index += 8)
    {
        currentData = *(uint64*)data;
        data += 8;
        m_v3 ^= currentData;
        for (size_t counter = 0; counter < numberOfRounds; ++counter)
        {
            SipRound();
        }
        m_v0 ^= currentData;
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SipHash::Finalize(size_t numberOfRounds)
{
    m_v2 ^= 0xFF;

    for (size_t counter = 0; counter < numberOfRounds; ++counter)
    {
        SipRound();
    }

    m_finalHash = m_v0 ^ m_v1 ^ m_v2 ^ m_v3;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SipHash::SipRound()
{
    m_v0 += m_v1;
    m_v1 <<= 13;
    m_v1 ^= m_v0;
    m_v0 <<= 32;
    m_v2 += m_v3;
    m_v3 <<= 16;
    m_v3 ^= m_v2;
    m_v2 += m_v1;
    m_v1 <<= 17;
    m_v1 ^= m_v2;
    m_v2 <<= 32;
    m_v0 += m_v3;
    m_v3 <<= 21;
    m_v3 ^= m_v0;
}

//#define SIPROUND                                                               \
//    do {                                                                       \
//        v0 += v1;                                                              \
//        v1 = ROTL(v1, 13);                                                     \
//        v1 ^= v0;                                                              \
//        v0 = ROTL(v0, 32);                                                     \
//        v2 += v3;                                                              \
//        v3 = ROTL(v3, 16);                                                     \
//        v3 ^= v2;                                                              \
//        v0 += v3;                                                              \
//        v3 = ROTL(v3, 21);                                                     \
//        v3 ^= v0;                                                              \
//        v2 += v1;                                                              \
//        v1 = ROTL(v1, 17);                                                     \
//        v1 ^= v2;                                                              \
//        v2 = ROTL(v2, 32);                                                     \
//    } while (0)
