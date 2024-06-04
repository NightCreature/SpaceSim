#pragma once

#include "Graphics/Effect.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <d3d11.h>
#include <vector>


class DeviceManager;
class Resource;
class EffectCache;
class Resource;
struct CommandList;

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
    }
    ~OrientationAxis() {}

    void cleanup();

    void initialise(Resource* resource, const DeviceManager& deviceManager, CommandList& list);
    void draw(const DeviceManager& deviceManager, const Matrix44& view, const Matrix44& projection, Resource* resource, CommandList& list);
private:

    size_t CreateConstantBuffer(size_t size, const DeviceManager& deviceManager, DescriptorHeap& heap, std::string_view name);
    void transform(const DeviceManager& deviceManager, const Matrix44& view, const Matrix44& projection);
    //static Color m_colorStream[];
    Effect* m_effect;
    MeshResourceIndices m_renderIndices;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    ConstantBuffer m_constantBuffer;
};
