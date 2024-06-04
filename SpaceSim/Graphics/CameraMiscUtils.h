#pragma once
#include "Input/InputAction.h"
#include "Input/InputSystem.h"

constexpr size_t moveForward = "move_forwards"_hash;
constexpr size_t moveBackWards = "move_backwards"_hash;
constexpr size_t moveLeft = "move_left"_hash;
constexpr size_t moveRight = "move_right"_hash;
constexpr size_t yawLeft = "move_yaw_left"_hash;
constexpr size_t yawRight = "move_yaw_right"_hash;
constexpr size_t pitchUp = "move_pitch_up"_hash;
constexpr size_t pitchDown = "move_pitch_down"_hash;
constexpr size_t rollLeft = "move_roll_left"_hash;
constexpr size_t rollRight = "move_roll_right"_hash;

inline InputActions::ActionType fowardAction;
inline InputActions::ActionType backwardAction;
inline InputActions::ActionType moveLeftAction;
inline InputActions::ActionType moveRightAction;
inline InputActions::ActionType yawLeftAction;
inline InputActions::ActionType yawRightAction;
inline InputActions::ActionType pitchUpAction;
inline InputActions::ActionType pitchDownAction;
inline InputActions::ActionType rollLeftAction;
inline InputActions::ActionType rollRightAction;

inline void SetupCameraInputActionTypes()
{
    InputSystem::getInputActionFromName(moveForward, fowardAction);
    InputSystem::getInputActionFromName(moveBackWards, backwardAction);
    InputSystem::getInputActionFromName(moveLeft, moveLeftAction);
    InputSystem::getInputActionFromName(moveRight, moveRightAction);
    InputSystem::getInputActionFromName(yawLeft, yawLeftAction);
    InputSystem::getInputActionFromName(yawRight, yawRightAction);
    InputSystem::getInputActionFromName(pitchUp, pitchUpAction);
    InputSystem::getInputActionFromName(pitchDown, pitchDownAction);
    InputSystem::getInputActionFromName(rollLeft, rollLeftAction);
    InputSystem::getInputActionFromName(rollRight, rollRightAction);
}