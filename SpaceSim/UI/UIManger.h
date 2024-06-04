#pragma once

#include <windows.h>

#include "UI/Core/StateManager.h"
#include "UI/Core/FlowManager.h"

class UIManger
{
public:
    UIManger() {}
    ~UIManger() {}

    void Initialise(Resource* resource);

    void Update(float deltaTime, const InputState& input);
private:
    Resource* m_resource = nullptr;
    FE::StateManager m_stateManager;
    FE::FlowManager m_flowManager;
    
};

