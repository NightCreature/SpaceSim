#include "Gameplay/ECS/Systems/SystemTypeRegistry.h"

namespace ECS
{


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void RegisterSystemType(const SystemType* type)
{
    auto& SystemRegister = GetSystemRegistry();

    SystemRegister.m_SystemTypes[type->GetId()] = type;
    SystemRegister.m_idToTag[type->GetId()] = type->GetSystemType();
    SystemRegister.m_idToCreationFp[type->GetId()] = type->GetCreateFP();
}

}