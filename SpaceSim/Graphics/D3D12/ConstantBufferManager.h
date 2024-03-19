#pragma once

#include "Core/Resource/RenderResource.h"
#include "Graphics\D3D12\ConstantBuffer.h"
#include <vector>

class Resource;

class PerFrameDataStorage
{
public:
    void Initialise(Resource* resource) { m_resource = resource; }
    void Destroy();

    template<class T>
    size_t CreateCBInterface(const T& data);

    ConstantBuffer& GetConstantBuffer(size_t index) { return m_constantBuffers[index]; }

private:
    static constexpr size_t PerFrameDataStorageAmount = 32;
    std::array<ConstantBuffer, PerFrameDataStorageAmount> m_constantBuffers;
    size_t m_usedBuffers = 0;
    Resource* m_resource = nullptr;
};

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
template<class T>
size_t PerFrameDataStorage::CreateCBInterface(const T& data)
{
    UNUSEDPARAM(data);
    if (m_resource == nullptr)
    {
        return m_usedBuffers;
    }
    m_constantBuffers[m_usedBuffers] = ConstantBuffer();
    RenderResource* renderResource = static_cast<RenderResource*>(m_resource);
    m_constantBuffers[m_usedBuffers].Create(renderResource->getDeviceManager(), renderResource->getDescriptorHeapManager().GetSRVCBVUAVHeap(), sizeof(T));


    return m_usedBuffers++;
}

