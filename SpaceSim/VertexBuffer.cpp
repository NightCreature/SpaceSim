#include "VertexBuffer.h"
#include "DeviceManager.h"
#include <sstream>
#include <iostream>

#include "../SpaceSim/StringHelperFunctions.h"

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool VertexBuffer::createBufferAndLayoutElements(const DeviceManager& deviceManager, unsigned int bufferSize, void* data, bool dynamic, const VertexDecalartionDesctriptor& vertexDeclartion, const ID3DBlob* vertexShaderCodeBlob)
{
    D3D11_BUFFER_DESC bufferDescriptor;
    ZeroMemory(&bufferDescriptor, sizeof(D3D11_BUFFER_DESC));
    bufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDescriptor.ByteWidth = bufferSize;
    bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDescriptor.CPUAccessFlags = 0;
    bufferDescriptor.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
    initData.pSysMem = data;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    HRESULT hr = deviceManager.getDevice()->CreateBuffer( &bufferDescriptor, &initData, &m_buffer );
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("VERTEXBUFFER", "Failed to create a D3D11 Buffer object with code: 0x%x", hr );
        return false;
    }

    if (!createVertexInputLayout(deviceManager, const_cast<ID3DBlob*>(vertexShaderCodeBlob), createInputElementLayout(vertexDeclartion)))
    {
        MSG_TRACE_CHANNEL("VERTEXBUFFER", "Failed to create Vertex Input Layout" );
        return false;
    }
    
    m_vertexCount = bufferSize / m_vertexStride;
    dynamic = false;

    return true;
}
//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool VertexBuffer::createVertexInputLayout( const DeviceManager& deviceManager, ID3DBlob* vertexShaderCodeBlob, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements )
{
    HRESULT hr = deviceManager.getDevice()->CreateInputLayout(&inputElements[0], (unsigned int)inputElements.size(), vertexShaderCodeBlob->GetBufferPointer(), vertexShaderCodeBlob->GetBufferSize(), &m_inputLayout );

    if (FAILED( hr ) )
    {        
        MSG_TRACE_CHANNEL("VERTEXBUFFER", "Failed to create the input layout: 0x%x", hr )
            return false;
    }

    return true;
}
//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> VertexBuffer::createInputElementLayout( const VertexDecalartionDesctriptor& vertexDeclartion )
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDataLayoutElements;
    //Create the buffer layout elements
    unsigned int numberOfElements = 0;
    if (vertexDeclartion.position)
    {
        ++numberOfElements;
    }
    if (vertexDeclartion.normal)
    {
        ++numberOfElements;
    }
    numberOfElements += (unsigned int)vertexDeclartion.textureCoordinateDimensions.size();
    vertexDataLayoutElements.reserve(numberOfElements);//New overwrites data on the stack
    m_vertexStride = 0;
    if (vertexDeclartion.position)
    {
        D3D11_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "POSITION";
        layout.SemanticIndex = 0; 
        layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout.InputSlot = 0;
        layout.AlignedByteOffset = m_vertexStride;
        layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        vertexDataLayoutElements.push_back(layout);
        m_vertexStride += 12;
    }

    if (vertexDeclartion.normal)
    {
        D3D11_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "NORMAL";
        layout.SemanticIndex = 0; 
        layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout.InputSlot = 0;
        layout.AlignedByteOffset = m_vertexStride;
        layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        vertexDataLayoutElements.push_back(layout);
        m_vertexStride += 12;
    }

    if (vertexDeclartion.vertexColor)
    {
        D3D11_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "COLOR";
        layout.SemanticIndex = 0; 
        layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout.InputSlot = 0;
        layout.AlignedByteOffset = m_vertexStride;
        layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        vertexDataLayoutElements.push_back(layout);
        m_vertexStride += 16;
    }

    for (unsigned int counter = 0; counter < vertexDeclartion.textureCoordinateDimensions.size(); ++counter)
    {
        D3D11_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "TEXCOORD";
        layout.SemanticIndex = counter; 
        layout.InputSlot = 0;
        layout.AlignedByteOffset = m_vertexStride;
        layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        if (vertexDeclartion.textureCoordinateDimensions[counter] == 2)
        {
            layout.Format = DXGI_FORMAT_R32G32_FLOAT;
            m_vertexStride += 8;
        }
        else if (vertexDeclartion.textureCoordinateDimensions[counter] == 3)
        {
            layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            m_vertexStride += 12;
        }
        else if (vertexDeclartion.textureCoordinateDimensions[counter] == 4)
        {
            layout.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            m_vertexStride += 16;
        }
        else
        {
            //Assume 2D texcoords
            layout.Format = DXGI_FORMAT_R32G32_FLOAT;
            m_vertexStride += 8;
        }
        vertexDataLayoutElements.push_back(layout);
        
    }

    return vertexDataLayoutElements;
}

