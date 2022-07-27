#pragma once

#include "StringOperations/StringHelperFunctions.h"
#include "Types/TemplateTypeMagic.h"

#include <any>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>
#include <array>


template<class T, class ...Ts>
bool find()
{
    return std::is_base_of_v<std::true_type, decltype(recursiveIsInPack<T, Ts...>())>;
}

void print();


class ConstructionHelperInterface
{
public:
    virtual ~ConstructionHelperInterface() {}

    virtual std::any  ConstructInstance() { return (void*)nullptr; }
};

template<class T>
class ConstructionHelper : public ConstructionHelperInterface
{
public:
    using TypeName = T;

    std::any ConstructInstance() override { return TypeName(); }
};

template<class T>
T Create() { return T(); }



class ConstructionFactory
{
public:
    template<class T>
    void RegisterDataTypeForName(const std::string& name)
    {
        m_constructors.push_back(std::make_pair(hashString(name), new ConstructionHelper<T>));
    }

    std::any ConstructForName(const std::string& name);
private:
    std::vector<std::pair<size_t, ConstructionHelperInterface*>> m_constructors;
};

template <size_t index, typename T, typename... Types>
constexpr T constructTypeFromPack()
{
    static_assert(index <= sizeof...(Types), "Index is bigger than provided list of types");
    if constexpr (index == 0)
    {
        return T();
    }
    else
    {
        return constructTypeFromPack<index - 1, Types...>();
    }
}


//template <typename T, typename... Types>
//constexpr T constructTypeFromPack(size_t index)
//{
//	if (index == 0)
//	{
//		return T();
//	}
//	else
//	{
//		return constructTypeFromPack<Types...>(index - 1);
//	}
//}
//
//template <typename T, typename ...Types>
//constexpr auto selectFromPack(size_t index)
//{
//	if (index == 0)
//	{
//		return T();
//	}
//	else
//	{
//		return selectFromPack<Types...>(index - 1);
//	}
//}

template<class T, class ...U>
size_t FindIndexForTypeInPack()
{
    return 0;
}

template<class ...T>
struct TypesPack
{};

template<class ...T>
class VariantConstructionFactory
{
public:
    using variant_type = std::variant<T...>;
    using type_pack = TypesPack<T...>;

    template<class T>
    void RegisterNameForDataType(const std::string& name)
    {
        size_t hash = hashString(name);
        size_t index = FindTypeIndexForName(name);
        if (-1 == index) //Not sure we should do this and the line above
        {
            //Find index for type in pack
            size_t index = FindIndexForTypeInPack<T, type_pack()>();
            recursiveIsInPack<T, T...> recursiveSearch;
        }
        m_stringHashMapToTypeIndex[hash] = index;
    }

    size_t FindTypeIndexForName(const std::string& name)
    {
        auto iterator = m_stringHashMapToTypeIndex.find(hashString(name));
        if (iterator != m_stringHashMapToTypeIndex.end())
        {
            return iterator->second;
        }

        return -1;
    }

    variant_type ConstructForName(const std::string& name)
    {
        //size_t typeIndex = FindTypeIndexForName(name);
        //return constructTypeFromPack<T...>(typeIndex);
        return variant_type();
    }

private:
    
    std::vector<std::pair<size_t, variant_type>> m_constructors;
    std::unordered_map<size_t, size_t> m_stringHashMapToTypeIndex;
};
