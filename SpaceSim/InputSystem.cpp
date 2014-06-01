#include "InputSystem.h"
#include "XController.h"
#include "KeyboardController.h"
#include "StringHelperFunctions.h"

InputSystem::AvailableActions InputSystem::m_availableActions;

    //-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
InputSystem::InputSystem()
{
}
//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
IInputDevice* InputSystem::createController( const ControllerType type )
{
    IInputDevice* controller = nullptr;
    if (type == Gamepad)
    {
        controller =  new XInputDevice(0);
    }
    else if (type == Keyboard)
    {
        controller = new KeyboardInputDevice();        
    }

    controller->enableController();
    m_controllers.insert(ControllerAndStatePair(controller, controller->getState()));

    return controller;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void InputSystem::update( float elapsedTime, double time )
{
    for (ControllersAndStateIt it = m_controllers.begin(); it != m_controllers.end(); ++it)
    {
        IInputDevice* controller = (*it).first;
        (*it).second = controller->update();
    }

    elapsedTime = 0.0f;
    time = 0.0;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void InputSystem::initialise( const std::string& inputMapFileName )
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
            m_availableActions.emplace_back(InputActions::ActionType(actionNameAttribute->Value(), actionLngNameAttribute != nullptr ? actionLngNameAttribute->Value() : ""));
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
            controller->initialise();
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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

    return Gamepad;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
InputActions::ActionType InputSystem::getInputActionFromName( unsigned int actionName )
{
    InputActions::ActionType returnValue;

    for (AvailableActions::const_iterator it = m_availableActions.begin(); it != m_availableActions.end(); ++it)
    {
        if (it->getType() == actionName)
        {
            returnValue = *it;
            break;
        }
    }

    return returnValue;
}
