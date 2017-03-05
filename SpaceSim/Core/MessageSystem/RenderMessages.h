#pragma once

#include "Core/MessageSystem/Messages.h"

#include "Graphics/Light.h"

namespace MessageSystem
{

class CreateLightMessage : public Message
{
public:
    CreateLightMessage() { m_MessageId = MESSAGE_ID(CreateLightMessage); }

    Light m_light;
    std::string m_lightName;
};

}