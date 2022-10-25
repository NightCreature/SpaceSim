#pragma once

#include "StringHelperFunctions.h"
#include <string>

///-----------------------------------------------------------------------------
///! @brief Helper object that in debug also shows the string its based on
///! @remark constexpr usage prefered
///-----------------------------------------------------------------------------
class HashString
{
public:
    constexpr HashString(const std::string_view& str) :
#ifdef _DEBUG
        m_string(str),
#endif
        m_hash(hashString(str)) {}

    constexpr ~HashString() {}

    constexpr size_t getHash() const { return m_hash; }
    constexpr operator size_t() const { return m_hash; }
#ifdef _DEBUG
    constexpr const std::string_view& getString() const { return m_string; }
#endif
private:
#ifdef _DEBUG
    std::string_view m_string;
#endif
    size_t m_hash;
};

