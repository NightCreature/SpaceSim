#include "Gameplay/ECS/Components/ComponentTypeRegistry.h"

namespace ECS
{


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void RegisterComponentType(const ComponentType* type)
{
    auto& componentRegister = GetComponentRegistry();

    componentRegister.m_componentTypes[type->GetId()] = type;
    componentRegister.m_idToTag[type->GetId()] = type->GetComponentType();
    componentRegister.m_idToCreationFp[type->GetId()] = type->GetCreateFP();
}

}
