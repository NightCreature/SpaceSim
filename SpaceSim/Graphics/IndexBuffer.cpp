#include "Graphics/IndexBuffer.h"
#include "Graphics/DeviceManager.h"
#include <iostream>
#include "Core/StringOperations/StringHelperFunctions.h"

#include "Graphics/D3DDebugHelperFunctions.h"

void IndexBuffer::createBuffer( const DeviceManager& deviceManager, unsigned int bufferSize, void* data, bool dynamic )
{
    D3D11_BUFFER_DESC bufferDescriptor;
    ZeroMemory(&bufferDescriptor, sizeof(D3D11_BUFFER_DESC));
    bufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDescriptor.ByteWidth = bufferSize;
    bufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
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
        MSG_TRACE_CHANNEL("INDEXBUFFER", "Failed to create a D3D11 Buffer object with code: 0x%x", hr );
    }

    dynamic = false;

    D3DDebugHelperFunctions::SetDebugChildName(m_buffer, FormatString("Index buffer size: %d, no. indecis: %d", bufferSize, bufferSize / (sizeof(unsigned int))));
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void IndexBuffer::cleanup()
{
    if (m_buffer)
    {
        m_buffer->Release();
        m_buffer = 0;
    }
}