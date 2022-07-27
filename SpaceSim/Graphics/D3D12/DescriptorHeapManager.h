#pragma once

#include <D3D12.h>
#include <vector>

//THis needs optimisation and better management strategies because at the moment its one big list 

class Resource;

struct DescriptorHeap
{
    //These are in out params, if you pass invalidDescriptorIndex they will create one for you and give you the index in the heap back in the param you pass in
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(size_t& index);
    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(size_t index = -1);

    D3D12_DESCRIPTOR_HEAP_DESC m_descriptor;
    ID3D12DescriptorHeap* m_heap = nullptr;

    size_t GetCPUDescriptorHeapRange(size_t numberOfDescriptors);
    size_t GetCPUDescriptorHeapRangeRingBuffer(size_t numberOfDescriptors);
    void ResetRingBufferRange() { m_ringBufferUsed = 0; }

    //Probably need to double this for the GPU versions
    size_t m_incrementSize = 0;
    size_t m_maxNumberDescriptors = 0;
    size_t m_usedDescriptors = 0;

    //Ringbuffer area usage
    size_t m_ringbufferStart = 0;
    size_t m_ringBufferUsed = 0;

    static constexpr size_t invalidDescriptorIndex = static_cast<size_t>(-1);
    
};

class DescriptorHeapManager
{
public:
    DescriptorHeapManager() { }
    ~DescriptorHeapManager();
    void Initialise(Resource* resource);
    void Cleanup();
    
    void CreateHeaps();
    DescriptorHeap& GetSRVCBVUAVHeap() { return m_descriptorHeaps[0]; }
    DescriptorHeap& GetSamplerHeap() { return m_descriptorHeaps[1]; }

    DescriptorHeap& GetShaderVisibleHeap() { return m_shaderVisibleHeap; }

    DescriptorHeap CreateDescriptorHeap(size_t descriptorCount, D3D12_DESCRIPTOR_HEAP_TYPE type, bool shaderVisible, bool haveRingBufferSpace);
private:

    std::vector<DescriptorHeap> m_descriptorHeaps;
    DescriptorHeap m_shaderVisibleHeap;

    Resource* m_resource = nullptr;
};