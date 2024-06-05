#include "StateManager.h"

#include "Core/Resource/GameResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "UI/Core/States/Registry/StateRegistry.h"
#include "UI/Core/States/State.h"

#include "Core/tinyxml2.h"
#include "Logging/LoggingMacros.h"

namespace FE
{

constexpr std::string_view StatesElementName = "states";
const std::string ChannelName("StateManager");

StateManager::~StateManager()
{
    for (auto& statePair : m_states)
    {
        delete statePair.second;
    }
}

void StateManager::Initialise(Resource* resource)
{
    m_resource = resource;
}

bool StateManager::Serialise(const std::filesystem::path& filePath)
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
    tinyxml2::XMLElement* statesElement = doc.FirstChildElement(StatesElementName.data());
    if (statesElement == nullptr)
    {
        return false;
    }

    auto& stateRegistry = FE::States::GetStateRegistry();

    for (tinyxml2::XMLElement* stateElement = statesElement->FirstChildElement(); stateElement != nullptr; stateElement = stateElement->NextSiblingElement())
    {
        //lets see if this state can be constructed
        States::CreateStateFP createStateFp = nullptr;
        auto iterator = stateRegistry.m_idToCreationFp.find(stateElement->Value());
        if (iterator != stateRegistry.m_idToCreationFp.end())
        {
            createStateFp = iterator->second;
        }
        if (createStateFp != nullptr)
        {
            States::State* state = createStateFp(m_resource);
            if (state)
            {
                if (state->Serialise(stateElement))
                {
                    m_states.insert(std::make_pair(state->GetName(), state));
                }
                else
                {
                    MSG_ERROR_CHANNEL(ChannelName, "Failed to deserialise for state type: %s", stateElement->Value());
                    delete state; //Otherwise we get a memory leak
                }
            }
            else
            {
                MSG_ERROR_CHANNEL(ChannelName, "Failed to create an instance of: %s", stateElement->Value());
            }
        }
        else
        {
            MSG_ERROR_CHANNEL(ChannelName, "State: %s was not registered", stateElement->Value());
        }
    }

    return true;
}

FE::States::State* StateManager::GetStateForId(const HashString& stateId)
{
    if (m_states.contains(stateId))
    {
        return m_states[stateId];
    }

    return nullptr;
}

}