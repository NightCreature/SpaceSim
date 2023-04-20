#pragma once

#include <D3D12.h>
#include <vector>
#include "Core/Types/Types.h"
#include "Core/tinyxml2.h"
#include "D3D12/StructuredBuffer.h"
#include "Math/VectorTypes.h"
#include "Graphics/Color.h"
#include <map>
#include <variant>
#include "../bin/Shaders/Shaders/BindlessBuffers.h"

class DeviceManager;
struct CommandList;

//This is part of the PSO now, and need serialisation for the shaders that way we can just add this to the effect definition/shader
struct VertexDeclarationDescriptor
{
    VertexDeclarationDescriptor() :
        position(3),
        normal(false),
        tangent(false),
        vertexColor(false)
    {
        textureCoordinateDimensions.clear();
    }

    void Deserialise(const tinyxml2::XMLElement* node);

    size_t position;
    bool normal;
    bool tangent;
    bool vertexColor;
    std::vector<unsigned int> textureCoordinateDimensions;
};

class VertexBuffer2
{
public:
    VertexBuffer2(void){}
    ~VertexBuffer2(void) {}

    void cleanup()
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

    void CleanupUploadResource()
    {
        if (m_uploadResource != nullptr)
        {
            m_uploadResource->Release();
            m_uploadResource = nullptr;
        }
    }

    void Create(const DeviceManager& deviceManager, CommandList& commandList, size_t bufferSize, void* data, size_t vertexStride);

    const D3D12_VERTEX_BUFFER_VIEW& GetBufferView() const { return m_bufferView; }
    ID3D12Resource* GetGPUResource() const { return m_defaultResource; }
    ID3D12Resource* GetCPUResource() const { return m_uploadResource; }
private:
    ID3D12Resource* m_defaultResource = nullptr;
    ID3D12Resource* m_uploadResource = nullptr;
    D3D12_VERTEX_BUFFER_VIEW m_bufferView;
};

//The index here has to match the type in the variant
enum class VertexSourceType : size_t
{
    Single = 0,
    Double,
    Triple,
    Quadruple,
    Color
};

using VertexSourceDataStream = std::variant<
    std::vector<float>,
    std::vector<Vector2>,
    std::vector<Vector3>,
    std::vector<Vector4>,
    std::vector<Color>
>;

enum class VertexStreamType : size_t
{
    Position = 0,
    Normal,
    Tangent,
    Color,
    UVStart
};

struct VertexDataStreams
{
    std::map<VertexStreamType, VertexSourceDataStream> m_streams;
    size_t m_uvStreamCount = 0;
};

VertexDataStreams CreateDataStreams(const VertexDeclarationDescriptor& descriptor);

class VertexBuffer
{
public:
    MeshResourceIndices CreateBuffer(const DeviceManager& deviceManager, CommandList& commandList, DescriptorHeap& heap, const VertexDataStreams& data);
    void Destroy();
private:
    std::vector<std::pair< VertexStreamType, StructuredBuffer>> m_data; //Data arrays on GPU
};