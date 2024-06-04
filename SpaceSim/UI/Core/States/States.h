#pragma once

#include "UI/Core/States/Registry/StateRegistry.h"
#include "UI/Core/States/Screen.h"

namespace FE
{

namespace States
{

#define REGISTER_STATE(STATE) RegisterState<STATE>(#STATE);

inline void RegisterStates()
{
    REGISTER_STATE(Screen);
}

}

}