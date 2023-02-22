#include "RenderJobs.h"

#include "Core/Resource/RenderResource.h"


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void CompileShaderJob::Execute(size_t threadIndex)
{
    RenderResourceHelper renderResource = { m_resource };
    auto& effectCache = renderResource.getWriteableResource().getEffectCache();
    //Have to protect this now
    effectCache.createEffect(m_resource, m_fileName);
}

