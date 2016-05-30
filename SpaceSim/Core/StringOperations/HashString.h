#pragma once

#include "StringHelperFunctions.h"
#include <string>

class HashString
{
public:
    HashString(const std::string& str) : 
#ifdef _DEBUG
        m_string(str),
#endif
        m_hash(hashString(str)) {}
    ~HashString() {}

    unsigned int getHash() const { return m_hash; }
    operator unsigned int() const { return m_hash; }
#ifdef _DEBUG
    const std::string& getString() const { return m_string; }
#endif
private:
#ifdef _DEBUG
    std::string m_string;
#endif
    unsigned int m_hash;
};

