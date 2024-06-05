#pragma once

#include "UI/Core/Behaviours/Registry/BehaviourRegistry.h"
#include "UI/Core/Behaviours/StaticText.h"
#include "UI/Core/Behaviours/SelectObject.h"



namespace FE
{

namespace Behaviours
{

struct EmptyElement {};

#define REGISTER_BEHAVIOUR(BEHAVIOUR) RegisterBehaviour<BEHAVIOUR>(#BEHAVIOUR);

inline void RegisterBehaviours()
{
    REGISTER_BEHAVIOUR(StaticText);
    REGISTER_BEHAVIOUR(SelectObject);
}

}

}