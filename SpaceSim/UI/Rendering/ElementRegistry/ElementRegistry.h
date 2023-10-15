#pragma once

#include "Core/StringOperations/HashString.h"
#include "ElementType.h"

#include <unordered_map>

namespace FE
{

namespace Rendering
{

struct ElementRegistry
{
    using TypesById = std::unordered_map<HashString, const ElementType*>;
    using IdToElementTag = std::unordered_map<HashString, ElementTag>;
    using IdToCreationFp = std::unordered_map<HashString, CreateElementFP>;

    TypesById m_elementTypes;
    IdToElementTag m_idToTag;
    IdToCreationFp m_idToCreationFp;
};

inline static ElementRegistry theElementRegister;
inline ElementRegistry& GetElementRegistry() { return theElementRegister; }

void RegisterElementType(const ElementType* type)
{
    if (type != nullptr)
    {
        auto& elementRegister = GetElementRegistry();

        auto id = type->GetId();
        elementRegister.m_elementTypes[id] = type;
        elementRegister.m_idToTag[id] = type->GetElementType();
        elementRegister.m_idToCreationFp[id] = type->GetCreateFP();
    }
}

template<class Element>
ElementTag RegisterElement(const char* ElementName, ElementTag ElementTag = CreateUniqueTag())
{
    static DefaultElementType<Element> ElementType(ElementName, ElementTag);

    RegisterElementType(&ElementType);

    return ElementTag;
}

inline const ElementTag& GetElementTagForElement(const char* ElementName)
{
    auto& registery = GetElementRegistry();
    return registery.m_idToTag[ElementName];
}

inline const ElementType* GetElementTypeForElement(const char* ElementName)
{
    auto& registery = GetElementRegistry();
    return registery.m_elementTypes[ElementName];
}

inline Element* CreateElement(const char* ElementName)
{
    auto& registery = GetElementRegistry();
    return registery.m_idToCreationFp[ElementName]();
}

}

}