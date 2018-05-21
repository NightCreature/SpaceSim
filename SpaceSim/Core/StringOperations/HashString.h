#pragma once

#include "StringHelperFunctions.h"
#include <string>

///-----------------------------------------------------------------------------
///! @brief This is not the best use of a hashed string we need, this is not compile time setup
///! @remark
///-----------------------------------------------------------------------------
class HashString
{
public:
    HashString(const std::string& str) : 
#ifdef _DEBUG
        m_string(str),
#endif
        m_hash(hashString(str)) {}
    ~HashString() {}

    size_t getHash() const { return m_hash; }
    operator size_t() const { return m_hash; }
#ifdef _DEBUG
    const std::string& getString() const { return m_string; }
#endif
private:
#ifdef _DEBUG
    std::string m_string;
#endif
   size_t m_hash;
};

