#pragma once

#include <D3D12.h>
#include <vector>
#include "Core/tinyxml2.h"

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

    const std::vector<D3D12_INPUT_ELEMENT_DESC>& createInputElementLayout(size_t& vertexStride);

    size_t GetVertexStride() const;

    size_t position;
    bool normal;
    bool tangent;
    bool vertexColor;
    std::vector<unsigned int> textureCoordinateDimensions;

    std::vector<D3D12_INPUT_ELEMENT_DESC> m_vertexDataLayoutElements;
};

class VertexBuffer
{
public:
    VertexBuffer(void){}
    ~VertexBuffer(void)
    {
        cleanup();
    }

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
