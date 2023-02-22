#pragma once

#include "D3D12.h"
#include "D3D12X.h"
#include "Core/Types/Types.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"

#include <windows.h>
#include <combaseapi.h>
#include <Optick.h>
#include <span>

class StructuredBuffer
{
public:
    template<class T>
    void Create(const DeviceManager& deviceManager, CommandList& commandList, DescriptorHeap& heap, const std::vector<T>& data);
    void Destroy();

    ID3D12Resource* GetBufferResource() const { return m_defaultResource; }
    size_t GetDescriptorIndex() const { return m_heapIndex; }
private:
    ID3D12Resource* m_defaultResource = nullptr;
    ID3D12Resource* m_uploadResource = nullptr;
    size_t m_heapIndex = DescriptorHeap::invalidDescriptorIndex;
};

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
template <class T>
void StructuredBuffer::Create(const DeviceManager& deviceManager, CommandList& commandList, DescriptorHeap& heap, const std::vector<T>& data)
{
    //D3D12 resource creation
    D3D12_HEAP_PROPERTIES defaultHeap;
    defaultHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    defaultHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;
    defaultHeap.CreationNodeMask = 1;
    defaultHeap.VisibleNodeMask = 1;

    size_t byteSize = data.size() * sizeof(T);

    D3D12_RESOURCE_DESC resourceDefaultDescriptor;
    resourceDefaultDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDefaultDescriptor.Alignment = 0;
    resourceDefaultDescriptor.Width = byteSize; //data size
    resourceDefaultDescriptor.Height = 1;
    resourceDefaultDescriptor.DepthOrArraySize = 1;
    resourceDefaultDescriptor.MipLevels = 1;
    resourceDefaultDescriptor.Format = DXGI_FORMAT_UNKNOWN;
    resourceDefaultDescriptor.SampleDesc.Count = 1;
    resourceDefaultDescriptor.SampleDesc.Quality = 0;
    resourceDefaultDescriptor.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDefaultDescriptor.Flags = D3D12_RESOURCE_FLAG_NONE;


    D3D12_RESOURCE_DESC resourceUploadDescriptor;
    resourceUploadDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceUploadDescriptor.Alignment = 0;
    resourceUploadDescriptor.Width = byteSize; //data size
    resourceUploadDescriptor.Height = 1;
    resourceUploadDescriptor.DepthOrArraySize = 1;
    resourceUploadDescriptor.MipLevels = 1;
    resourceUploadDescriptor.Format = DXGI_FORMAT_UNKNOWN;
    resourceUploadDescriptor.SampleDesc.Count = 1;
    resourceUploadDescriptor.SampleDesc.Quality = 0;
    resourceUploadDescriptor.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceUploadDescriptor.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_HEAP_PROPERTIES uploadHeap;
    uploadHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    uploadHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
    uploadHeap.CreationNodeMask = 1;
    uploadHeap.VisibleNodeMask = 1;

    HRESULT hr = deviceManager.GetDevice()->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &resourceDefaultDescriptor, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_defaultResource));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("VertexBuffer", "Failed to create the GPU structured buffer resource");
    }

    hr = deviceManager.GetDevice()->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &resourceUploadDescriptor, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_uploadResource));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("VertexBuffer", "Failed to create the CPU/Upload structured buffer resource");
    }
    //Would need to fix this
    //D3DDebugHelperFunctions::SetDebugChildName(m_defaultResource, FormatString("Vertex Buffer with size: %d, no. vertices: %d", bufferSize, m_vertexCount));

    D3D12_SUBRESOURCE_DATA initData = {};
    //ZeroMemory(&initData, sizeof(D3D12_SUBRESOURCE_DATA));
    initData.pData = reinterpret_cast<void*>(const_cast<T*>(data.data()));
    initData.RowPitch = byteSize;
    initData.SlicePitch = byteSize;

    //First arg is command list
    UpdateSubresources<1>(commandList.m_list, m_defaultResource, m_uploadResource, 0, 0, 1, &initData);


    //Need to create a descriptor again here
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_defaultResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON);
    commandList.m_list->ResourceBarrier(1, &barrier);

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.NumElements = static_cast<UINT>(data.size());
    srvDesc.Buffer.StructureByteStride = sizeof(T);
    srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

    //This fails why?
    deviceManager.GetDevice()->CreateShaderResourceView(m_defaultResource, &srvDesc, heap.GetCPUDescriptorHandle(m_heapIndex));

#ifdef _DEBUG
    static size_t bufferCount = 0;
    std::wstringstream str;
    str << L"Structured Buffer" << bufferCount++;
    m_defaultResource->SetName(str.str().c_str());
    str << L"Upload";
    m_uploadResource->SetName(str.str().c_str());
#endif
}