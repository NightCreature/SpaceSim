#pragma once

#include "Core/StringOperations/HashString.h"

#include <filesystem>
#include <unordered_map>

class Resource;

namespace FE
{

namespace States
{

class State;

}

class StateManager
{
public:
    StateManager() {}
    ~StateManager();

    void Initialise(Resource* resource);
    bool Serialise(const std::filesystem::path& filePath);

    States::State* GetStateForId(const HashString& stateId);
private:
    Resource* m_resource = nullptr;
    using StatePair = std::pair<HashString, States::State*>;
    std::unordered_map<HashString, States::State*> m_states;
};

}