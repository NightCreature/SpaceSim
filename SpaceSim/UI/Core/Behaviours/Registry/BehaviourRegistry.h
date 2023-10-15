#pragma once

#include "Core/StringOperations/HashString.h"
#include "BehaviourType.h"

#include <unordered_map>

namespace FE
{

namespace Behaviours
{

struct BehaviourRegistry
{
    using TypesById = std::unordered_map<HashString, const BehaviourType*>;
    using IdToBehaviourTag = std::unordered_map<HashString, BehaviourTag>;
    using IdToCreationFp = std::unordered_map<HashString, CreateBehaviourFP>;

    TypesById m_BehaviourTypes;
    IdToBehaviourTag m_idToTag;
    IdToCreationFp m_idToCreationFp;
};

inline static BehaviourRegistry theBehaviourRegister;
inline BehaviourRegistry& GetBehaviourRegistry() { return theBehaviourRegister; }

inline void RegisterBehaviourType(const BehaviourType* type)
{
    if (type != nullptr)
    {
        auto& BehaviourRegister = GetBehaviourRegistry();

        auto id = HashString(type->GetId());
        BehaviourRegister.m_BehaviourTypes[id] = type;
        BehaviourRegister.m_idToTag[id] = type->GetBehaviourType();
        BehaviourRegister.m_idToCreationFp[id] = type->GetCreateFP();
    }
}

template<class Behaviour>
BehaviourTag RegisterBehaviour(const char* BehaviourName, BehaviourTag BehaviourTag = CreateUniqueTag())
{
    static DefaultBehaviourType<Behaviour> BehaviourType(BehaviourName, BehaviourTag);

    RegisterBehaviourType(&BehaviourType);

    return BehaviourTag;
}

inline const BehaviourTag& GetBehaviourTagForBehaviour(const char* BehaviourName)
{
    auto& registery = GetBehaviourRegistry();
    return registery.m_idToTag[BehaviourName];
}

inline const BehaviourType* GetBehaviourTypeForBehaviour(const char* BehaviourName)
{
    auto& registery = GetBehaviourRegistry();
    return registery.m_BehaviourTypes[BehaviourName];
}

inline Behaviour* CreateBehaviour(const char* BehaviourName)
{
    auto& registery = GetBehaviourRegistry();
    return registery.m_idToCreationFp[BehaviourName]();
}

inline CreateBehaviourFP GetBehaviourCreate(const char* behaviourName)
{
    CreateBehaviourFP function = nullptr;
    auto iterator = theBehaviourRegister.m_idToCreationFp.find(behaviourName);
    if (iterator != theBehaviourRegister.m_idToCreationFp.end())
    {
        function = iterator->second;
    }

    return function;
}

}

}