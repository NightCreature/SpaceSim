#include "DescriptorHeapManager.h"

#include "Core/Resource/RenderResource.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"
#include <d3d12.h>



///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
DescriptorHeapManager::~DescriptorHeapManager()
{
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void DescriptorHeapManager::Initialise(Resource* resource)
{
    m_resource = resource;
    

    //Create the shader visible heap and set it up so we can get segments fromt this heap
    //m_shaderVisibleHeap = CreateDescriptorHeap(2048, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, true);

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void DescriptorHeapManager::Cleanup()
{
    for (auto& heap : m_descriptorHeaps)
    {
        heap.m_heap->Release();
        heap.m_heap = nullptr;
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void DescriptorHeapManager::CreateHeaps()
{
    CreateDescriptorHeap(65536, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, true, true);
    CreateDescriptorHeap(256, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, true, false);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
DescriptorHeap DescriptorHeapManager::CreateDescriptorHeap(size_t descriptorCount, D3D12_DESCRIPTOR_HEAP_TYPE type, bool shaderVisible, bool haveRingBufferSpace)
{
    //Create the heap for the back buffers we still have to attach the resources
    DescriptorHeap heap;
    heap.m_descriptor.NumDescriptors = static_cast<UINT>(descriptorCount);
    heap.m_descriptor.Type = type;
    heap.m_descriptor.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE: D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    heap.m_descriptor.NodeMask = 0;

    auto& resourceWrapper = RenderResourceHelper(m_resource).getWriteableResource();
    auto* device = resourceWrapper.getDeviceManager().GetDevice();
    

    HRESULT hr = device->CreateDescriptorHeap(&heap.m_descriptor, IID_PPV_ARGS(&heap.m_heap));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("DescriptorHeapManager", "Failed to create a descriptor heap of size %d", descriptorCount);
    }

    heap.m_incrementSize = static_cast<size_t>(device->GetDescriptorHandleIncrementSize(heap.m_descriptor.Type));
    heap.m_maxNumberDescriptors = haveRingBufferSpace ? descriptorCount / 2 : descriptorCount;
    heap.m_usedDescriptors = 0;
    heap.m_ringbufferStart = descriptorCount / 2;
    heap.m_ringBufferUsed = 0;

    m_descriptorHeaps.push_back(heap);
    return heap;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This should create a GPU and CPU handle at the same time
///-----------------------------------------------------------------------------
D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUDescriptorHandle(size_t& index)
{
    D3D12_CPU_DESCRIPTOR_HANDLE handle(m_heap->GetCPUDescriptorHandleForHeapStart());
    if (index == invalidDescriptorIndex)
    {
        handle.ptr += m_usedDescriptors * m_incrementSize;
        index = m_usedDescriptors;
        ++m_usedDescriptors;
    }
    else
    {
        handle.ptr += index * m_incrementSize;
    }
    return handle;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This should create a GPU and CPU handle at the same time
///-----------------------------------------------------------------------------
D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUDescriptorHandle(size_t index)
{
    D3D12_GPU_DESCRIPTOR_HANDLE handle(m_heap->GetGPUDescriptorHandleForHeapStart());
    if (index == invalidDescriptorIndex)
    {
        handle.ptr += m_usedDescriptors * m_incrementSize;
        ++m_usedDescriptors;
    }
    else
    {
        handle.ptr += index * m_incrementSize;
    }
    return handle;
}

///-----------------------------------------------------------------------------
///! @brief   Same as above except that these are not in the ring buffer so you can't easily clear them
///! @remark
///-----------------------------------------------------------------------------
size_t DescriptorHeap::GetCPUDescriptorHeapRange(size_t numberOfDescriptors)
{
    D3D12_GPU_DESCRIPTOR_HANDLE handle(m_heap->GetGPUDescriptorHandleForHeapStart());
    size_t retVal = m_usedDescriptors * m_incrementSize;
    handle.ptr = retVal;
    m_usedDescriptors += numberOfDescriptors;

    return retVal;
}

///-----------------------------------------------------------------------------
///! @brief   Function used to get a range of descriptors you can copy too
///! @remark
///-----------------------------------------------------------------------------
size_t DescriptorHeap::GetCPUDescriptorHeapRangeRingBuffer(size_t numberOfDescriptors)
{
    size_t retVal = static_cast<size_t>(-1);
    if (m_ringbufferStart + numberOfDescriptors + m_ringBufferUsed < m_maxNumberDescriptors)
    {
        //We can give back the index to the beginning of this range
        retVal = m_ringbufferStart + m_ringBufferUsed;
        m_ringBufferUsed += numberOfDescriptors;
    }

    return retVal;
}

