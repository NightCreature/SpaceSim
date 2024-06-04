#pragma once

#include <unordered_map>

#include "StateType.h"

class Resource;

namespace FE
{

namespace States
{

struct StateRegistry
{
    using TypesById = std::unordered_map<HashString, const StateType*>;
    using IdToStateTag = std::unordered_map<HashString, StateTag>;
    using IdToCreationFp = std::unordered_map<HashString, CreateStateFP>;

    TypesById m_StateTypes;
    IdToStateTag m_idToTag;
    IdToCreationFp m_idToCreationFp;
};

inline static StateRegistry theStateRegister;
inline StateRegistry& GetStateRegistry() { return theStateRegister; }

inline void RegisterStateType(const StateType* type)
{
    if (type != nullptr)
    {
        auto& StateRegister = GetStateRegistry();

        auto id = HashString(type->GetId());
        StateRegister.m_StateTypes[id] = type;
        StateRegister.m_idToTag[id] = type->GetStateType();
        StateRegister.m_idToCreationFp[id] = type->GetCreateFP();
    }
}

template<class State>
StateTag RegisterState(const char* StateName, StateTag StateTag = CreateUniqueTag())
{
    static DefaultStateType<State> StateType(StateName, StateTag);

    RegisterStateType(&StateType);

    return StateTag;
}

inline const StateTag& GetStateTagForState(const HashString& StateName)
{
    auto& registery = GetStateRegistry();
    return registery.m_idToTag[StateName];
}

inline const StateType* GetStateTypeForState(const HashString& StateName)
{
    auto& registery = GetStateRegistry();
    return registery.m_StateTypes[StateName];
}

inline State* CreateState(const HashString& StateName, Resource* resource)
{
    auto& registery = GetStateRegistry();
    return registery.m_idToCreationFp[StateName](resource);
}

}

}