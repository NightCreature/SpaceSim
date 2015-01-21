#pragma once
#include "IController.h"

class MouseController : public IInputDevice
{
public:
    MouseController();
    virtual ~MouseController();

    virtual void initialise() override;
    virtual const InputState& update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput) override;
    virtual const bool isConnected() const override;
    virtual void enableController() override;
    virtual void disableController() override;
private:
    virtual void internalActionSetup(InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input) override;

};

