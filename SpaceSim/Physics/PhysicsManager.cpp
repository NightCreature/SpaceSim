#include <Physics/PhysicsManager.h>

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void PhysicsManager::AddColidableBbox(Bbox* bbox)
{
    m_staticBoundingBoxes.push_back(bbox);
}
