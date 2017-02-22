#pragma once

#include "Core/Types/Types.h"

class TaskContext
{
public:
    virtual ~TaskContext() {}
};

class SortContext : public TaskContext
{
public:
    RenderInstanceTree* m_renderInstances;
};

class SortRenderInstancesTask
{
public:
    SortRenderInstancesTask() {}

    void Initialise(TaskContext* context) 
    {
        SortContext* sort_context = static_cast<SortContext*>(context); 
        m_renderInstances = sort_context->m_renderInstances; 
    }
    void Run()
    {
        BROFILER_CATEGORY("SortRenderInstancesTask::Run", Profiler::Color::DarkRed);
        std::sort(begin(*m_renderInstances), end(*m_renderInstances), [=](const RenderInstance* lhs, const RenderInstance* rhs)
        {
            return lhs->getShaderInstance().getMaterial().getBlendState() < rhs->getShaderInstance().getMaterial().getBlendState();
        });

        std::sort(begin(*m_renderInstances), end(*m_renderInstances), [=](const RenderInstance* lhs, const RenderInstance* rhs)
        {
            const Material& lhsMaterial = lhs->getShaderInstance().getMaterial();
            const Material& rhsMaterial = rhs->getShaderInstance().getMaterial();
            //This might not be the most effiecient material/sahder id combo
            return lhsMaterial.getEffect()->getTechnique(lhsMaterial.getTechnique())->getTechniqueId() < rhsMaterial.getEffect()->getTechnique(rhsMaterial.getTechnique())->getTechniqueId();
        });
    }

private:
    RenderInstanceTree* m_renderInstances;
};