#pragma once

#include "Graphics/Effect.h"
#include "Graphics/VertexBuffer.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <d3d11.h>

class DeviceManager;
class Resource;
class EffectCache;
class Resource;

class OrientationAxis
{
public:
    struct LineVertex
    {
        float x,y,z;
        //DWORD vertexColor;
    };


    OrientationAxis() :
        m_effect()
    {
        m_wvpConstants.m_world.identity();
        m_wvpConstants.m_view.identity();
        m_wvpConstants.m_projection.identity();
    }
    ~OrientationAxis() {}

    void cleanup();

    void initialise(Resource* resource, const DeviceManager& deviceManager);
    void draw(const DeviceManager& deviceManager, Resource* resource);
private:
    void transform(const DeviceManager& deviceManager);
    static LineVertex m_vertices[];
    Effect* m_effect;
    VertexBuffer m_vertexBuffer;
    WVPBufferContent m_wvpConstants;
};
