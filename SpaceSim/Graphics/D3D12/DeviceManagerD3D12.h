#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <d3d12.h>
#include <vector>

///-----------------------------------------------------------------------------
///! @brief  Manager to manage d3d device or OGL context and the CG context when CG shaders are used
///! @remark
///-----------------------------------------------------------------------------
class DeviceManagerD3D12
{
public:
    DeviceManagerD3D12();
    ~DeviceManagerD3D12() { cleanup(); }
    void clearDeviceState()
    {
    }
    void cleanup();
    bool createDevice();

    ID3D12Device* getDevice() const { return m_device; }
    //This dont exist in D3D12, so they return null at this point better to hard crash
    void* getDeviceContext() const { return nullptr; }
    void* getDeferredDeviceContext() const { return nullptr; }

    //HACKS to see if this works
    void setDevice(ID3D12Device* device) { m_device = device; }
    //This doesnt exist
    void setDeviceContext(void* deviceContext) { UNUSEDPARAM(deviceContext); }
    D3D_FEATURE_LEVEL getFreatureLevel() const { return m_featureLevel; }

    IDXGISwapChain* GetSwapChain() { return m_swapChain; }
    bool createSwapChain(HWND windowHandle, int windowWidth, int windowHeight);

private:

    bool InitialiseDebugLayers();
    bool InitialiseDXGIFactory();

    IDXGIAdapter* EnumerateAdapters();

    ID3D12Device* m_device = nullptr;
    ID3D12CommandQueue* m_commandQueue = nullptr;
    D3D_FEATURE_LEVEL m_featureLevel = static_cast<D3D_FEATURE_LEVEL>(0);

    IDXGIFactory* m_dxgiFactory = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;
    DXGI_SWAP_CHAIN_DESC m_swapChainDescriptor;

#ifdef _DEBUG
    ID3D12Debug* m_debugInterface = nullptr;
#endif

    size_t m_frameCount = 3;

    unsigned int m_qualityLevels = 0;
};
