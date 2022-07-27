#pragma once

#include "Core/Types/TypeHelpers.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Graphics/D3D12/DescriptorHeapManager.h"

#include <d3d12.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>
#include <vector>

class Resource;

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>


///-----------------------------------------------------------------------------
///! @brief  Manager to manage d3d device or OGL context and the CG context when CG shaders are used
///! @remark
///-----------------------------------------------------------------------------
class DeviceManager
{
public:
    DeviceManager() = default;
    ~DeviceManager() { cleanup(); }
    void clearDeviceState()
    {
    }
    void cleanup();
    void Initialise(Resource* resource) { m_resource = resource; }

    bool createDevice();

    ID3D11Device* getDevice() const { return nullptr; }// m_device; }
    ID3D12Device6* GetDevice() const { return m_device; }
    //This dont exist in D3D12, so they return null at this point better to hard crash
    ID3D11DeviceContext* getDeviceContext() const { return nullptr; }
    ID3D11DeviceContext* getDeferredDeviceContext() const { return nullptr; }

    //HACKS to see if this works
    void setDevice(ID3D12Device6* device) { m_device = device; }
    //This doesnt exist
    void setDeviceContext(void* deviceContext) { UNUSEDPARAM(deviceContext); }
    D3D_FEATURE_LEVEL getFreatureLevel() const { return m_featureLevel; }

    IDXGISwapChain* GetSwapChain() { return m_swapChain; }
    bool createSwapChain(HWND windowHandle, int windowWidth, int windowHeight);

    

    size_t GetSwapChainCommandQueueIndex() { return m_swapChainCommandQueueIndex; }
    size_t GetSwapChainCommandListIndex() { return m_swapChainCommandListIndex; }

    D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilHandle();
    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferRTVHandle();
    ID3D12Resource* GetCurrentBackBuffer();


    size_t m_fenceValue;

    void SetCurrentFrameBufferIndex(size_t frameBufferIndex) { m_currentBackBufferIndex = frameBufferIndex;  m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex(); }
private:
    bool InitialiseDebugLayers();
    bool InitialiseDXGIFactory();
    bool CheckFeatures();
    IDXGIAdapter* EnumerateAdapters();

    std::vector<ID3D12Resource*> m_backBufferRenderTargets;
    ID3D12Resource* m_depthStencil = nullptr;

    ID3D12Device6* m_device = nullptr;
    D3D_FEATURE_LEVEL m_featureLevel = static_cast<D3D_FEATURE_LEVEL>(0);

    IDXGIFactory7* m_dxgiFactory = nullptr;
    IDXGISwapChain4* m_swapChain = nullptr;
    DXGI_SWAP_CHAIN_DESC1 m_swapChainDescriptor;

    //Descriptor heaps
    DescriptorHeap m_renderTargetViewDescriptorHeap;
    DescriptorHeap m_depthStencilDescriptorHeap;
    DescriptorHeap m_srvUAVDescriptorHeap;
    DescriptorHeap m_samplerDescriptorHeap;

    Resource* m_resource = nullptr;

    size_t m_rtvDescriptorSize = 0;
    size_t m_depthStencilDescriptorSize = 0;

//#ifdef _DEBUG
    ID3D12Debug* m_debugInterface = nullptr;
//#endif

    size_t m_swapChainCommandQueueIndex = 0;
    size_t m_swapChainCommandListIndex = 0;
    size_t m_frameCount = 3;
    size_t m_currentBackBufferIndex = 0;
    size_t depthStencilDescriptorIndex = DescriptorHeap::invalidDescriptorIndex;

    unsigned int m_qualityLevels = 0;
};
