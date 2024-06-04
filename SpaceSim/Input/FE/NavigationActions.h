#pragma once

#include "Input/InputAction.h"
#include "Input/InputSystem.h"

struct NavigationActions
{
    [[nodiscard]]
    NavigationActions();

    static InputActions::ActionType feUp;
    static InputActions::ActionType feDown;
    static InputActions::ActionType feLeft;
    static InputActions::ActionType feRight;
    static InputActions::ActionType feAccept;
    static InputActions::ActionType feCancel;
};

