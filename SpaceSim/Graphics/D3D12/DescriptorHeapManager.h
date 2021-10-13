#pragma once

#include <D3D12.h>
#include <vector>

//THis needs optimisation and better management strategies because at the moment its one big list 

class Resource;

struct DescriptorHeap
{
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(size_t index = -1);
    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(size_t index = -1);

    D3D12_DESCRIPTOR_HEAP_DESC m_descriptor;
    ID3D12DescriptorHeap* m_heap = nullptr;

    //Probably need to double this for the GPU versions
    size_t m_incrementSize = 0;
    size_t m_maxNumberDescriptors = 0;
    size_t m_usedDescriptors = 0;
};

class DescriptorHeapManager
{
public:
    DescriptorHeapManager() { }
    ~DescriptorHeapManager();
    void Initialise(Resource* resource) { m_resource = resource; }
    
    DescriptorHeap CreateDescriptorHeap(size_t descriptorCount, D3D12_DESCRIPTOR_HEAP_TYPE type);

private:
    std::vector<DescriptorHeap> m_descriptorHeaps;

    Resource* m_resource = nullptr;
};