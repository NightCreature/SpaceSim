#include "Input/IController.h"
#include "Input/InputSystem.h"
#include "Core/Types/TypeHelpers.h"
#include "Core/StringOperations/HashString.h"
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void IInputDevice::deserialise( const tinyxml2::XMLElement* element, const InputSystem& inputSystem )
{
    UNUSEDPARAM(inputSystem);
    unsigned int inputActionHash = hashString("InputAction");
    for (; element; element = element->NextSiblingElement())
    {
        if (hashString(element->Value()) == inputActionHash)
        {
            const tinyxml2::XMLAttribute* inputActionAttr = element->FindAttribute("input_action");
            if (inputActionAttr != nullptr)
            {
                HashString hash(inputActionAttr->Value());
				InputActions::ActionType inputActionType;
				inputSystem.getInputActionFromName(hash.getHash(), inputActionType);
                StandardInputAction inputAction(inputActionType);
                m_controllerState.addStandardAction(inputAction);
                for (const tinyxml2::XMLElement* inputElement = element->FirstChildElement(); inputElement; inputElement = inputElement->NextSiblingElement())
                {
                    const tinyxml2::XMLAttribute* inputAttr = inputElement->FindAttribute("input");
                    if (inputAttr != nullptr)
                    {
                        internalActionSetup(inputActionType, inputAttr);
                    }
                    else
                    {
                        MSG_TRACE_CHANNEL("InputDevice error", "Either the input(0x%x) is not defined", inputAttr);
                    }
                }
            }
            else
            {
                MSG_TRACE_CHANNEL("InputDevice error", "Either the input_action(0x%x) is not defined", inputActionAttr);
            }
        }
    }
}