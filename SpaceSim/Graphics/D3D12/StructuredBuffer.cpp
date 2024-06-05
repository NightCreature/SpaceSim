#include "Graphics/D3D12/StructuredBuffer.h"


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void StructuredBuffer::Destroy()
{
    if (m_defaultResource != nullptr)
    {
        m_defaultResource->Release();
    }

    if (m_uploadResource != nullptr)
    {
        m_uploadResource->Release();
    }
}
