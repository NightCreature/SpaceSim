#include "Core/ConstructionFactory.h"
#include "Logging/LoggingMacros.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
std::any ConstructionFactory::ConstructForName(const std::string& name)
{
    size_t hash = Hashing::hashString(name);

    auto iterator = std::find_if(m_constructors.begin(), m_constructors.end(), [hash](auto constructionPair) { return constructionPair.first == hash; });
    if (iterator != m_constructors.end())
    {
        return iterator->second->ConstructInstance();
    }

    return nullptr;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void print()
{
    //auto temp = recursiveIsInPack<std::string, double, std::variant<int, bool, float, double>, int>();
    if (find<std::variant<int, bool, float, double>, double, std::variant<int, bool, float, double>, int>())
    {
        MSG_TRACE_CHANNEL("ConstructionFactory", "recursive type list search found the answer");
        std::stringstream str;
        auto x = 42u;
        auto y = 42;
        str << nameof<size_t>() << ", " << nameof<decltype(x)>() << ", " << nameof<decltype(y)>();
        MSG_TRACE_CHANNEL("ConstructionFactory", "%s", str.str().c_str());


    }
    else
    {
        MSG_TRACE_CHANNEL("ConstructionFactory", "recursive type list search something doesnt work");
    }

    auto pack = SearchableTypelist< double, std::variant<int, bool, float, double>, int>();
    if (pack.contains(nameof<int>))
    {
        MSG_TRACE_CHANNEL("ConstructionFactory", "Pack Find constexpr");
        size_t index = pack.index(nameof<int>);
        std::stringstream str;
        str << nameof<int>();
        MSG_TRACE_CHANNEL("ConstructionFactory", "Index of the type (%s) in the pack is: %d",  str.str().c_str(), index);
    }

    //auto pair = MakePair<int>(nameof<int>);
}
