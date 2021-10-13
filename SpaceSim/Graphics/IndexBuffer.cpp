#include "Graphics/IndexBuffer.h"

#include "Graphics/DeviceManager.h"
#include <iostream>
#include "Core/StringOperations/StringHelperFunctions.h"

#include "Graphics/D3DDebugHelperFunctions.h"

#include "Graphics/D3D12/CommandQueue.h"
#include "Graphics/D3D12/D3D12X.h"

//void IndexBuffer::createBuffer( const DeviceManager& deviceManager, unsigned int bufferSize, void* data, bool dynamic )
//{
//    D3D11_BUFFER_DESC bufferDescriptor;
//    ZeroMemory(&bufferDescriptor, sizeof(D3D11_BUFFER_DESC));
//    bufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
//    bufferDescriptor.ByteWidth = bufferSize;
//    bufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    bufferDescriptor.CPUAccessFlags = 0;
//    bufferDescriptor.MiscFlags = 0;
//
//    D3D11_SUBRESOURCE_DATA initData;
//    ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
//    initData.pSysMem = data;
//    initData.SysMemPitch = 0;
//    initData.SysMemSlicePitch = 0;
//
//    HRESULT hr = deviceManager.getDevice()->CreateBuffer( &bufferDescriptor, &initData, &m_buffer );
//    if (FAILED(hr))
//    {
//        MSG_TRACE_CHANNEL("INDEXBUFFER", "Failed to create a D3D11 Buffer object with code: 0x%x", hr );
//    }
//
//    dynamic = false;
//
//    D3DDebugHelperFunctions::SetDebugChildName(m_buffer, FormatString("Index buffer size: %d, no. indecis: %d", bufferSize, bufferSize / (sizeof(unsigned int))));
//}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void IndexBuffer::cleanup()
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

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void IndexBuffer::Create(const DeviceManager& deviceManager, CommandList& commandList, size_t bufferSize, void* data)
{
    //D3D12 resource creation
    D3D12_HEAP_PROPERTIES defaultHeap;
    defaultHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    defaultHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;
    defaultHeap.CreationNodeMask = 1;
    defaultHeap.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDescriptor;
    resourceDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDescriptor.Alignment = 0;
    resourceDescriptor.Width = bufferSize; //data size
    resourceDescriptor.Height = 1;
    resourceDescriptor.DepthOrArraySize = 1;
    resourceDescriptor.MipLevels = 1;
    resourceDescriptor.Format = DXGI_FORMAT_UNKNOWN;
    resourceDescriptor.SampleDesc.Count = 1;
    resourceDescriptor.SampleDesc.Quality = 0;
    resourceDescriptor.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDescriptor.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_HEAP_PROPERTIES uploadHeap;
    uploadHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    uploadHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
    uploadHeap.CreationNodeMask = 1;
    uploadHeap.VisibleNodeMask = 1;

    HRESULT hr = deviceManager.GetDevice()->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &resourceDescriptor, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_defaultResource));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("IndexBuffer", "Failed to create the GPU index buffer resource");
    }
    hr = deviceManager.GetDevice()->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &resourceDescriptor, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_uploadResource));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("IndexBuffer", "Failed to create the CPU/Upload index buffer resource");
    }

    // Copy data to the intermediate upload heap and then schedule a copy 
    // from the upload heap to the index buffer.
    D3D12_SUBRESOURCE_DATA indexData = {};
    indexData.pData = data;
    indexData.RowPitch = bufferSize;
    indexData.SlicePitch = indexData.RowPitch;

    UpdateSubresources<1>(commandList.m_list, m_defaultResource, m_uploadResource, 0, 0, 1, &indexData);

    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_defaultResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
    commandList.m_list->ResourceBarrier(1, &barrier);

    // Describe the index buffer view.
    m_bufferView.BufferLocation = m_defaultResource->GetGPUVirtualAddress();
    m_bufferView.Format = DXGI_FORMAT_R32_UINT;
    m_bufferView.SizeInBytes = static_cast<unsigned int>(bufferSize);

    m_numberOfIndecis = static_cast<unsigned int>(bufferSize / sizeof(unsigned int));
 }
