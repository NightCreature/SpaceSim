#include "ConstantBufferManager.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PerFrameDataStorage::Destroy()
{
    for (auto& cb : m_constantBuffers)
    {
        cb.Destroy();
    }
}
