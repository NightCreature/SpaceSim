#pragma once
#include "Buffer.h"
#include <D3D11.h>
#include <vector>

class DeviceManager;

struct VertexDecalartionDesctriptor
{
    VertexDecalartionDesctriptor() :
        position(true),
        normal(false),
        vertexColor(false)
    {
        textureCoordinateDimensions.clear();
    }

    const std::vector<D3D11_INPUT_ELEMENT_DESC> createInputElementLayout() const;

    bool position;
    bool normal;
    bool vertexColor;
    std::vector<unsigned int> textureCoordinateDimensions;
};

class VertexBuffer
{
public:
    VertexBuffer(void) : m_inputLayout(0), m_buffer(0), m_vertexStride(0) {}
    ~VertexBuffer(void)
    {
        cleanup();
    }

    void cleanup()
    {
        if (m_buffer)
        {
            m_buffer->Release();
            m_buffer = 0;
        }

        if (m_inputLayout)
        {
            m_inputLayout->Release();
            m_inputLayout = 0;
        }
    }

    bool createBufferAndLayoutElements(const DeviceManager& deviceManager, unsigned int bufferSize, void* data, bool dynamic, const VertexDecalartionDesctriptor& vertexDeclartion, const ID3DBlob* vertexShaderCodeBlob);

    ID3D11Buffer* getBuffer() const { return m_buffer; }
    unsigned int getVertexStride() const { return m_vertexStride; }
    ID3D11InputLayout* getInputLayout() const { return m_inputLayout; }
    unsigned int getVertexCount() const { return m_vertexCount; }
private:
    bool createVertexInputLayout(const DeviceManager& deviceManager, ID3DBlob* vertexShaderCodeBlob, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements);
    
    ID3D11InputLayout* m_inputLayout;
    ID3D11Buffer* m_buffer;

    unsigned int m_vertexStride;
    unsigned int m_vertexCount;
};
