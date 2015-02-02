#pragma once

#include "StringHelperFunctions.h"
#include <string>

class HashString
{
public:
    HashString(const std::string& str) : m_string(str), m_hash(hashString(str)) {}
    ~HashString() {}

    unsigned int getHash() const { return m_hash; }
#ifdef _DEBUG
    const std::string& getString() const { return m_string; }
#endif
private:
#ifdef _DEBUG
    std::string m_string;
#endif
    unsigned int m_hash;
};

