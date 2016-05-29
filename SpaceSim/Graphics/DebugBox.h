#pragma once

#include "model.h"

namespace DebugGraphics
{

struct ColorVertex
{
    float x,y,z;
    float a,r,g,b;
};

class DebugBox
{
public:
    DebugBox(Resource* resource, const Vector3& lowerLeft, const Vector3& upperRight);
    virtual ~DebugBox();

    virtual void initialise( const ShaderInstance& shaderInstance );
private:
    Vector3 m_lowerLeft;
    Vector3 m_upperRight;
    Resource* m_resource;
    Model* box;
};

}
