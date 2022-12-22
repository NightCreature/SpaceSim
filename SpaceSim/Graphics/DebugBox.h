#pragma once

#include "model.h"

struct CommandList;

namespace DebugGraphics
{

struct ColorVertex
{
    float x,y,z;
    float a,r,g,b;
};

class DebugBox : public DebugRenderInterface
{
public:
    DebugBox(Resource* resource, const Vector3& lowerLeft, const Vector3& upperRight);
    virtual ~DebugBox();

    virtual void initialise();

    void UpdateCbs() override;
    void PopulateCommandlist(Resource* resource, CommandList& commandList) override;
    void Update(const MessageSystem::RenderInformation::RenderInfo& context) override;

private:
    Vector3 m_lowerLeft;
    Vector3 m_upperRight;
    Resource* m_resource;
    Model* box;
};

}
