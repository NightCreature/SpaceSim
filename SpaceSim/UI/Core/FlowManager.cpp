#include "FlowManager.h"

#include "Input/InputState.h"
#include "UI/Core/States/State.h"
#include "UI/Transistion.h"
#include "UI/Core/StateManager.h"
#include "Core/Resource/GameResource.h"

namespace FE
{

constexpr std::string_view flowTag = "flow";
constexpr std::string_view nodeTag = "node";
constexpr std::string_view idTag = "id";
constexpr std::string_view stateTag = "state";
constexpr std::string_view transitionTag = "transition";
constexpr std::string_view targetTag = "target";
constexpr std::string_view ChannelName = "FlowManager";
constexpr HashString InvalidId;

void FlowManager::Initialise(Resource* resource, StateManager* stateManager)
{
    m_resource = resource;
    m_stateManager = stateManager;
}

bool FlowManager::Serialise(const std::filesystem::path& filePath)
{
    GameResourceHelper helper(m_resource);
    const Paths* paths = helper.getResource().m_paths;

    tinyxml2::XMLDocument doc;

    auto fileName = filePath;
    if (!fileName.is_absolute())
    {
        fileName = paths->getFEPath() / filePath;
    }

    if (doc.LoadFile(fileName.string().c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("Texture Pack Loader Error", "Failed to load %s\nWith error: %d\nError string 1: %s\n Error string 2: %s", fileName.string().c_str(), doc.ErrorID(), doc.GetErrorStr1(), doc.GetErrorStr2());
        return false;
    }

    //Document is loaded lets skip the <xml> element
    tinyxml2::XMLElement* flowElement = doc.FirstChildElement(flowTag.data());
    if (flowElement == nullptr)
    {
        return false;
    }

    
    for (tinyxml2::XMLElement* nodeElement = flowElement->FirstChildElement(); nodeElement != nullptr; nodeElement = nodeElement->NextSiblingElement())
    {
        //Extract node attributes
        FlowNode node;
        for (const tinyxml2::XMLAttribute* attribute = nodeElement->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
        {
            if (attribute->Name() == idTag)
            {
                node.m_id = attribute->Value();
            }
            else if (attribute->Name() == stateTag)
            {
                node.m_state = m_stateManager->GetStateForId(attribute->Value());
                if (node.m_state == nullptr)
                {
                    MSG_ERROR_CHANNEL(ChannelName.data(), "Failed to get state for: %s", attribute->Value());
                    //return false;
                }
            }
        }

        //Read transition children
        for (tinyxml2::XMLElement* transitionElement = nodeElement->FirstChildElement(); transitionElement != nullptr; transitionElement = transitionElement->NextSiblingElement())
        {
            if (transitionElement->Name() == transitionTag.data())
            {
                HashString transitionId;
                const char* idAttributeValue = transitionElement->Attribute(idTag.data());
                if (idAttributeValue != nullptr)
                {
                    transitionId = idAttributeValue;
                }
                HashString transitionTarget;
                const char* targetAttributeValue = transitionElement->Attribute(targetTag.data());
                if (targetAttributeValue != nullptr)
                {
                    transitionTarget = targetAttributeValue;
                }

                node.m_transitions.push_back(std::make_pair(transitionId, transitionTarget));
            }
        }

        if (node.m_id != InvalidId)
        {
            m_flowMap.push_back(std::make_pair(node.m_id, node));
        }
        else
        {
            MSG_ERROR_CHANNEL(ChannelName.data(), "Failed to create valid flow node");
        }
    }

    return true;
}

void FlowManager::ActivateFlow()
{
    m_activeState = FindFlowNodeForId("1"); //This should be something better
}

void FlowManager::Update(float deltaTime, const InputState& input)
{
    if (m_activeState != nullptr)
    {
        m_activeState->m_state->Update(deltaTime, input);

        //Ask the active state if it does a transition and find the next state to activate, this might be a fly through state so we run through a bunch of them potentially
        FindNextUpdatingState(*m_activeState);
    }
}

void FlowManager::FindNextUpdatingState(const FlowNode& state)
{
    const Transition* transition = state.m_state->GetTransition();
    if (transition != nullptr)
    {
        if (!transition->IsBack())
        {
            const HashString& transitionId = transition->GetTransitionId();
            const HashString& nextTarget = state.GetTransitionForId(transitionId);
            auto* nextFlowNode = FindFlowNodeForId(nextTarget);
            if (nextFlowNode != nullptr)
            {

                //Interogate the state for its capabilities
                if (!nextFlowNode->m_state->RequiresUpdate()) //THis might need a check for a terminating state
                {
                    //Need to ask for its transition again until we hit one that needs an update
                    FindNextUpdatingState(*nextFlowNode);
                }
                else
                {
                    m_previousState = m_activeState;
                    m_activeState = nextFlowNode;
                }
            }
        }
        else
        {
            //This probably needs more logic
            auto* temp = m_previousState;
            m_activeState = m_previousState;
            m_previousState = temp;

        }
    }
}

FlowManager::FlowNode* FlowManager::FindFlowNodeForId(const HashString& id)
{
    auto iterator = std::find_if(std::begin(m_flowMap), std::end(m_flowMap), [id](const std::pair<HashString, FlowNode>& nodePair) { return id == nodePair.first; });
    if (iterator != std::end(m_flowMap))
    {
        return &(iterator->second);
    }

    return nullptr;
}

const HashString& FlowManager::FlowNode::GetTransitionForId(const HashString& id) const
{
    auto iterator = std::find_if(std::begin(m_transitions), std::end(m_transitions), [id](const std::pair<HashString, HashString>& transitionPair) { return transitionPair.first == id; });
    if (std::end(m_transitions) != iterator)
    {
        return iterator->second;
    }

    return InvalidId;
}

}
