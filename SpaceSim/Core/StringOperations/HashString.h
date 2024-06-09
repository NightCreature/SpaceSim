#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"

#include <string>
#include <string_view>


///-----------------------------------------------------------------------------
///! @brief Helper object that in debug also shows the string its based on
///! @remark constexpr usage prefered
///-----------------------------------------------------------------------------
class HashString
{
public:
    constexpr HashString() {}
    constexpr HashString(const char* string) : HashString(std::string_view(string)) { }
    constexpr HashString(const std::string_view& str) :
#ifdef _DEBUG
        m_string(str),
#endif
        m_hash(Hashing::hashString(str)) {}
    explicit HashString(size_t hash) : m_hash(hash) {}

    constexpr ~HashString() {}

    constexpr size_t getHash() const { return m_hash; }
    constexpr operator size_t() const { return m_hash; }

    constexpr const std::string_view& getString() const 
    { 
#ifdef _DEBUG
        return m_string;
#else
        return fmt::format("{::#x}", m_hash); //should print the 0xm_hash but its a string view sadly
#endif
    }


    constexpr bool equals(size_t nameHash) const { return nameHash == m_hash; }
    constexpr bool equals(const HashString& nameHash) const { return m_hash == nameHash.getHash(); }

    constexpr size_t operator()() const { return m_hash; }

private:
#ifdef _DEBUG
    std::string_view m_string;
#endif
    size_t m_hash = static_cast<size_t>(-1);
};

#define HASH_ELEMENT_DEFINITION(CLASS) static constexpr HashString m_hash = HashString(#CLASS);
#define HASH_ELEMENT_TEMPLATE_DEFINITION(CLASS, T) static constexpr HashString m_hash = HashString(#CLASS#T##);

// Specialization so we can use this type as a hash key in unordered map
template<>
struct std::hash<HashString>
{
    std::size_t operator()(const HashString& s) const noexcept
    {
        return s.getHash();
    }
};

inline bool operator==(const HashString& lhs, size_t rhs)
{
    return lhs.equals(rhs);
}

inline bool operator==(size_t lhs, const HashString& rhs)
{
    return rhs.equals(lhs);
}

inline bool operator==(const HashString& rhs, const HashString& lhs)
{
    return lhs.equals(rhs);
}

///-----------------------------------------------------------------------------
///! @brief User defined hash literal, hashes the string with constexpr
///! @remark
///-----------------------------------------------------------------------------
inline constexpr HashString operator ""_hashstring(const char* str, size_t size)
{
    return HashString(std::string_view(str, size));
}