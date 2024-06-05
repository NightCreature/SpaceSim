#include "NavigationActions.h"

InputActions::ActionType NavigationActions::feUp;
InputActions::ActionType NavigationActions::feDown;
InputActions::ActionType NavigationActions::feLeft;
InputActions::ActionType NavigationActions::feRight;
InputActions::ActionType NavigationActions::feAccept;
InputActions::ActionType NavigationActions::feCancel;

NavigationActions::NavigationActions()
{
    //<ActionName name = "fe_up" lng = "lng_fe_up" / >
    //<ActionName name = "fe_down" lng = "lng_fe_down" / >
    //<ActionName name = "fe_left" lng = "lng_fe_left" / >
    //<ActionName name = "fe_right" lng = "lng_fe_right" / >
    //<ActionName name = "fe_accept" lng = "lng_fe_accept" / >
    //<ActionName name = "fe_cancel" lng = "lng_cancel" / >

    InputSystem::getInputActionFromName("fe_up"_hash, feUp);
    InputSystem::getInputActionFromName("fe_down"_hash, feDown);
    InputSystem::getInputActionFromName("fe_left"_hash, feLeft);
    InputSystem::getInputActionFromName("fe_right"_hash, feRight);
    InputSystem::getInputActionFromName("fe_accept"_hash, feAccept);
    InputSystem::getInputActionFromName("fe_cancel"_hash, feCancel);
}