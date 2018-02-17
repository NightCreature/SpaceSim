#include "Input/InputSystem.h"
#include "Input/XController.h"
#include "Input/KeyboardController.h"
#include "Input/MouseController.h"
#include "Core/StringOperations/StringHelperFunctions.h"

InputSystem::AvailableActions InputSystem::m_availableActions;
#ifdef DEBUG
std::map < InputActions::ActionType, std::string > InputSystem::m_actionNames;
#endif

std::vector<RAWINPUT> InputSystem::m_rawKeyBoardInput;
std::vector<RAWINPUT> InputSystem::m_rawMouseInput;
std::vector<RAWINPUT> InputSystem::m_rawHidInput;

    ///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
InputSystem::InputSystem()
{
}
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
InputSystem::~InputSystem()
{
    ControllersAndStateIt it = m_controllers.begin();
    while (it != m_controllers.end())
    {
        delete (*it).first;
        it = m_controllers.erase(it);
    }
    m_controllers.clear();
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
IInputDevice* InputSystem::createController(const ControllerType type)
{
    IInputDevice* controller = nullptr;
    if (type == Gamepad)
    {
        controller = new XInputDevice(0);
    }
    else if (type == Keyboard)
    {
        controller = new KeyboardInputDevice();
    }
    else if (type == Mouse)
    {
        controller = new MouseController();
    }

    if (nullptr != controller)
    {
        controller->enableController();
        m_controllers.insert(ControllerAndStatePair(controller, controller->getState()));
    }

    return controller;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void InputSystem::update( float elapsedTime, double time )
{
    for (ControllersAndStateIt it = m_controllers.begin(); it != m_controllers.end(); ++it)
    {
        IInputDevice* controller = (*it).first;
        (*it).second = controller->update(m_rawKeyBoardInput, m_rawMouseInput, m_rawHidInput);
    }

	m_rawHidInput.clear();
	m_rawMouseInput.clear();
	m_rawKeyBoardInput.clear();

    elapsedTime = 0.0f;
    time = 0.0;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void InputSystem::initialise( const std::string& inputMapFileName, HWND hwnd )
{
    tinyxml2::XMLDocument actionsDoc;
    std::string availableActionsFileName = extractPathFromFileName(inputMapFileName) + "AvailableActions.xml";
    if (actionsDoc.LoadFile(availableActionsFileName.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("Input system Error", "Failed to load the input mapping file %s", availableActionsFileName.c_str());
    }

    const tinyxml2::XMLElement* element1 = actionsDoc.FirstChildElement();
    element1 = element1->FirstChildElement();//Skip the <xml> node

    for (; element1; element1 = element1->NextSiblingElement())
    {
        const tinyxml2::XMLAttribute* actionNameAttribute = element1->FindAttribute("name");
        const tinyxml2::XMLAttribute* actionLngNameAttribute = element1->FindAttribute("lng");
        if (actionLngNameAttribute != nullptr)
        {
            MSG_TRACE_CHANNEL("Input SYSTEM", "adding actions for: %s with hash %u", actionNameAttribute->Value(), hashString(actionNameAttribute->Value()));
            m_availableActions.emplace_back(InputActions::ActionType(actionNameAttribute->Value(), actionLngNameAttribute != nullptr ? actionLngNameAttribute->Value() : ""));
#ifdef DEBUG
			m_actionNames.insert(std::make_pair(m_availableActions[m_availableActions.size() - 1],actionNameAttribute->Value()));
#endif
        }
    }

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(inputMapFileName.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("Input system Error", "Failed to load the input mapping file %s", inputMapFileName.c_str());
    }

    const tinyxml2::XMLElement* element = doc.FirstChildElement();
    element = element->FirstChildElement();//Skip the <xml> node

    for (; element; element = element->NextSiblingElement())
    {
        IInputDevice* controller = createController(stringToControllerType(element->Name()));
        MSG_TRACE_CHANNEL("INPUT SYSTEM", "controller name: %s", element->Name());
        const tinyxml2::XMLAttribute* attribute = element->FindAttribute("input_map");
        if (controller != nullptr && attribute != nullptr )
        {
            tinyxml2::XMLDocument inputDoc;
            std::string buttonMapFileName = std::string(extractPathFromFileName(inputMapFileName) + attribute->Value());
            if (inputDoc.LoadFile(buttonMapFileName.c_str()) != tinyxml2::XML_NO_ERROR)
            {
                MSG_TRACE_CHANNEL("Input system Error", "Failed to load the input mapping file %s", buttonMapFileName.c_str());
                return;
            }

            tinyxml2::XMLElement* inputElement = inputDoc.FirstChildElement();
            inputElement = inputElement->FirstChildElement();//Skip the <xml> node
            controller->deserialise(inputElement, *this);
            controller->initialise(hwnd);
        }
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
ControllerType InputSystem::stringToControllerType( const std::string& controllerName )
{
    size_t controllerNameHash = hashString(controllerName);
    if (controllerNameHash == XInputDevice::m_hash)
    {
        return Gamepad;
    }
    else if (controllerNameHash ==  KeyboardInputDevice::m_hash)
    {
        return Keyboard;
    } 
    else if (controllerNameHash == MouseController::m_hash)
    {
        return Mouse;
    }

    return Gamepad;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
//InputActions::ActionType InputSystem::getInputActionFromName( unsigned int actionName )
bool InputSystem::getInputActionFromName(unsigned int actionName, InputActions::ActionType& actionType)
{
    for (AvailableActions::const_iterator it = m_availableActions.begin(); it != m_availableActions.end(); ++it)
    {
        if (it->getType() == actionName)
        {
			actionType = *it;
			return true;
        }
    }

    //MSG_TRACE_CHANNEL("INPUT SYSTEM", "Failed to find action for key %u", actionName);
    return false;
}
