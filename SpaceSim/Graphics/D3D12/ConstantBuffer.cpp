#include "Graphics/D3D12/ConstantBuffer.h"

static size_t numberOfConstantBuffers = 0;

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ConstantBuffer::Create(const DeviceManager& deviceManager, DescriptorHeap& heap, size_t size)
{
    m_cpuSideData = ConstantBufferData(size);
    m_cpuSideData.Create();

    D3D12_HEAP_PROPERTIES uploadHeap;
    uploadHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    uploadHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
    uploadHeap.CreationNodeMask = 1;
    uploadHeap.VisibleNodeMask = 1;


    D3D12_RESOURCE_DESC resourceDescriptor;
    resourceDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDescriptor.Alignment = 0;
    resourceDescriptor.Width = m_cpuSideData.m_size;
    resourceDescriptor.Height = 1;
    resourceDescriptor.DepthOrArraySize = 1;
    resourceDescriptor.MipLevels = 1;
    resourceDescriptor.Format = DXGI_FORMAT_UNKNOWN;
    resourceDescriptor.SampleDesc.Count = 1;
    resourceDescriptor.SampleDesc.Quality = 0;
    resourceDescriptor.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDescriptor.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT hr = deviceManager.GetDevice()->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &resourceDescriptor, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_constantBuffer));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("CONSTANT BUFFER", "Failed to create constant buffer with error: %d %s", hr, getLastErrorMessage(hr));
    }

    // Map and initialize the constant buffer. We don't unmap this until the
    // app closes. Keeping things mapped for the lifetime of the resource is okay.
    CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    hr = m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_GPUDataBegin));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("CONSTANT BUFFER", "Failed to map constant buffer with error: %d %s", hr, getLastErrorMessage(hr));
    }
        
    // Describe and create a constant buffer view.
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = static_cast<UINT>(m_cpuSideData.m_size);

    
    deviceManager.GetDevice()->CreateConstantBufferView(&cbvDesc, heap.GetCPUDescriptorHandle(m_heapIndex));

#ifdef _DEBUG
    std::wstringstream str;
    str << L"ConstantBuffer" << numberOfConstantBuffers++ << L"sized:" << m_cpuSideData.m_size;
    m_constantBuffer->SetName(str.str().c_str());
#endif
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ConstantBuffer::Destroy()
{
    m_constantBuffer->Release();
    m_heapIndex = DescriptorHeap::invalidDescriptorIndex;
    m_cpuSideData.Destroy();
}
