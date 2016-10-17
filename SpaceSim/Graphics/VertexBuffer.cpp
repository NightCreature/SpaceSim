#include "Graphics/VertexBuffer.h"
#include "Graphics/DeviceManager.h"
#include <sstream>
#include <iostream>

#include "Core/StringOperations/StringHelperFunctions.h"

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool VertexBuffer::createBufferAndLayoutElements(const DeviceManager& deviceManager, size_t bufferSize, void* data, bool dynamic, const VertexDeclarationDescriptor& vertexDeclartion, const ID3DBlob* vertexShaderCodeBlob)
{
    D3D11_BUFFER_DESC bufferDescriptor;
    ZeroMemory(&bufferDescriptor, sizeof(D3D11_BUFFER_DESC));
    bufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDescriptor.ByteWidth = (unsigned int)bufferSize;
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

    if (!createVertexInputLayout(deviceManager, const_cast<ID3DBlob*>(vertexShaderCodeBlob), vertexDeclartion.createInputElementLayout(m_vertexStride)))
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
const std::vector<D3D11_INPUT_ELEMENT_DESC> VertexDeclarationDescriptor::createInputElementLayout(size_t& vertexStride) const
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDataLayoutElements;
    //Create the buffer layout elements
    unsigned int numberOfElements = 0;
    if (position > 0)
    {
        ++numberOfElements;
    }
    if (normal)
    {
        ++numberOfElements;
    }
    numberOfElements += (unsigned int)textureCoordinateDimensions.size();
    vertexDataLayoutElements.reserve(numberOfElements);//New overwrites data on the stack
    vertexStride = 0;
    if (position > 0)
    {
        D3D11_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "POSITION";
        layout.SemanticIndex = 0; 
        switch (position)
        {
        case 1:
            layout.Format = DXGI_FORMAT_R32_FLOAT;
            break;
        case 2:
            layout.Format = DXGI_FORMAT_R32G32_FLOAT;
            break;
        default:
        case 3:
            layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            break;
        case 4:
            layout.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            break;
        }
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        vertexDataLayoutElements.push_back(layout);
        vertexStride += sizeof(float) * position;
    }

    if (normal)
    {
        D3D11_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "NORMAL";
        layout.SemanticIndex = 0; 
        layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        vertexDataLayoutElements.push_back(layout);
        vertexStride += 12;
    }

    if (tangent)
    {
        D3D11_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "TANGENT";
        layout.SemanticIndex = 0;
        layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        vertexDataLayoutElements.push_back(layout);
        vertexStride += 12;
    }

    if (vertexColor)
    {
        D3D11_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "COLOR";
        layout.SemanticIndex = 0; 
        layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        vertexDataLayoutElements.push_back(layout);
        vertexStride += 16;
    }

    for (unsigned int counter = 0; counter < textureCoordinateDimensions.size(); ++counter)
    {
        D3D11_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "TEXCOORD";
        layout.SemanticIndex = counter; 
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        if (textureCoordinateDimensions[counter] == 2)
        {
            layout.Format = DXGI_FORMAT_R32G32_FLOAT;
            vertexStride += 8;
        }
        else if (textureCoordinateDimensions[counter] == 3)
        {
            layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            vertexStride += 12;
        }
        else if (textureCoordinateDimensions[counter] == 4)
        {
            layout.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            vertexStride += 16;
        }
        else
        {
            //Assume 2D texcoords
            layout.Format = DXGI_FORMAT_R32G32_FLOAT;
            vertexStride += 8;
        }
        vertexDataLayoutElements.push_back(layout);
        
    }

    return vertexDataLayoutElements;
}

