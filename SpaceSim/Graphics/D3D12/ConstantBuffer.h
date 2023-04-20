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

struct ConstantData
{
    ConstantData() = default;
    ConstantData(size_t size) : m_size(CalculateCBSize(size)) {}

    size_t CalculateCBSize(size_t size)
    {
        return 256 * ((size / 256) + 1);
    }

    void Create()
    {
        m_data = new byte[m_size];
    }

    void Destroy()
    {
        delete[] m_data;
    }

    size_t m_size = 0;
    byte* m_data = nullptr;
};

template<class T>
ConstantData CreateConstantBufferData(const T& data)
{
    ConstantData retval(sizeof(T));
    memcpy(retval.m_data, static_cast<const byte*>(&data), sizeof(T));
    return retval;
}

class ConstantBuffer
{
public:
    void Create(const DeviceManager& deviceManager, DescriptorHeap& heap, size_t size);
    void Destroy();
    void UpdateGpuData() 
    {
        OPTICK_EVENT();
        memcpy(m_GPUDataBegin, m_cpuSideData.m_data, m_cpuSideData.m_size); 
    }

    template<class T>
    void UpdateCpuData(const T& data) 
    {
        OPTICK_EVENT();
        memcpy(m_cpuSideData.m_data, &data, sizeof(T));
    }

    template<class T, size_t ElementCount>
    void UpdateCpuData(const T (&data)[ElementCount])
    {
        OPTICK_EVENT();
        memcpy(m_cpuSideData.m_data, &data, sizeof(T) * ElementCount);
    }

    ID3D12Resource* GetConstantBuffer() const { return m_constantBuffer; }
    size_t GetHeapIndex() const { return m_heapIndex; }
private:
    ConstantData m_cpuSideData = {0};
    ID3D12Resource* m_constantBuffer = nullptr;
    byte* m_GPUDataBegin = nullptr;
    size_t m_heapIndex = DescriptorHeap::invalidDescriptorIndex;
};
