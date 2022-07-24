#pragma once

#include "D3D12.h"
#include "D3D12X.h"
#include "Core/Types/Types.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"

#include <windows.h>
#include <combaseapi.h>

struct ConstantBufferData
{
    ConstantBufferData() {}
    ConstantBufferData(size_t size)
    {
        m_size = CalculateCBSize(size);
        m_data = new byte[m_size];
    }
    ~ConstantBufferData()
    {
        m_size = 0;
        delete[] m_data;
        m_data = nullptr;
    }

    size_t CalculateCBSize(size_t size)
    {
        return 256 * ((size / 256) + 1);
    }


    size_t m_size = 0;
    byte* m_data = nullptr;
};

template<class T>
ConstantBufferData CreateConstantBufferData(const T& data)
{
    ConstantBufferData retval(sizeof(T));
    memcpy(retval.m_data, static_cast<const byte*>(&data), sizeof(T));
    return retval;
}

class ConstantBuffer
{
public:
    void Create(const DeviceManager& deviceManager, DescriptorHeap& heap, size_t size);
    void UpdateGpuData() 
    {
        memcpy(m_GPUDataBegin, m_cpuSideData.m_data, m_cpuSideData.m_size); 
    }

    template<class T>
    void UpdateCpuData(const T& data) 
    {
        memcpy(m_cpuSideData.m_data, &data, sizeof(T));
    }
    ID3D12Resource* GetConstantBuffer() const { return m_constantBuffer; }
private:
    ConstantBufferData m_cpuSideData = {};
    ID3D12Resource* m_constantBuffer = nullptr;
    byte* m_GPUDataBegin = nullptr;
    size_t m_heapIndex = DescriptorHeap::invalidDescriptorIndex;
};
