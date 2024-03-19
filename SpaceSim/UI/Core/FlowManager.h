#pragma once

#include "Core/StringOperations/HashString.h"

#include <filesystem>

class Resource;
class InputState;

namespace FE
{

class StateManager;

namespace States
{
class State;
}

class FlowManager
{
public:
    void Initialise(Resource* resource, StateManager* stateManager);
    bool Serialise(const std::filesystem::path& filePath);

    void ActivateFlow();

    void Update(float deltaTime, const InputState& input);

    void PrintFlowVisualisation(const std::filesystem::path& fileName);
private:
    struct FlowNode
    {
        HashString m_id;
        States::State* m_state = nullptr;
        std::vector<std::pair<HashString, HashString>> m_transitions;

        const HashString& GetTransitionForId(const HashString& id) const;
    };

    void FindNextUpdatingState(const FlowNode& state);
    FlowNode* FindFlowNodeForId(const HashString& id);

    std::vector<std::pair<HashString, FlowNode>> m_flowMap; //This is id mapped to actual node, it has a double id for ease of use

    Resource* m_resource;
    StateManager* m_stateManager;

    FlowNode* m_previousState = nullptr;
    FlowNode* m_activeState = nullptr;
};

}