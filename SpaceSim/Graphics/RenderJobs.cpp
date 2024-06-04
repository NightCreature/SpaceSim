#include "RenderJobs.h"

#include "Core/Resource/RenderResource.h"
#include "Core/Thread/JobSystem.h"


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool CompileShaderJob::Execute(ThreadContext* context)
{
    RenderResourceHelper renderResource = { context->m_renderResource };
    auto& effectCache = renderResource.getWriteableResource().getEffectCache();
    //Have to protect this now
    //effectCache.createEffect(context->m_renderResource, m_fileName);

    return true;
}

