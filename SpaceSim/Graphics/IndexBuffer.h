#pragma once

//#include <D3D11.h>
#include <d3d12.h>

class DeviceManager;
struct CommandList;

class IndexBuffer
{
public:
    IndexBuffer(void) {}
    ~IndexBuffer(void) { cleanup(); }

    void cleanup();

    void setNumberOfIndecis(unsigned int numberOfIndecis) { m_numberOfIndecis = numberOfIndecis; }
    unsigned int getNumberOfIndecis() const { return m_numberOfIndecis; }

    void CleanupUploadResource()
    {
        if (m_uploadResource != nullptr)
        {
            m_uploadResource->Release();
            m_uploadResource = nullptr;
        }

        if (m_defaultResource != nullptr)
        {
            m_defaultResource->Release();
        }
    }

    void Create(const DeviceManager& deviceManager, CommandList& commandList, size_t bufferSize, void* data);


    const D3D12_INDEX_BUFFER_VIEW& GetBufferView() const { return m_bufferView; }
    ID3D12Resource* GetGPUResource() const { return m_defaultResource; }
    ID3D12Resource* GetCPUResource() const { return m_uploadResource; }
private:
    ID3D12Resource* m_defaultResource = nullptr;
    ID3D12Resource* m_uploadResource = nullptr;
    D3D12_INDEX_BUFFER_VIEW m_bufferView;
    unsigned int m_numberOfIndecis;
};
