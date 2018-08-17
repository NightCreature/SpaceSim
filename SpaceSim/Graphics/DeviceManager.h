#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <d3d11.h>
#include <vector>

///-----------------------------------------------------------------------------
///! @brief  Manager to manage d3d device or OGL context and the CG context when CG shaders are used
///! @remark
///-----------------------------------------------------------------------------
class DeviceManager 
{
public:
    DeviceManager();;
    ~DeviceManager() { cleanup();  }
    void clearDeviceState()
    {
        if( m_deviceContext )
        {
            m_deviceContext->ClearState();
        }
    }
    void cleanup();
    bool createDevice();
    ID3D11Device* getDevice() const { return m_device; }
    ID3D11DeviceContext* getDeviceContext() const { return m_deviceContext; }

    //HACKS to see if this works
    void setDevice(ID3D11Device* device) { m_device = device; }
    void setDeviceContext(ID3D11DeviceContext* deviceContext) { m_deviceContext = deviceContext; }
    D3D_FEATURE_LEVEL getFreatureLevel() const { return m_featureLevel; }

    IDXGISwapChain* GetSwapChain() { return m_swapChain; }
    bool createSwapChain(HWND windowHandle, int windowWidth, int windowHeight);

private:
    ID3D11Device* m_device;
    D3D_FEATURE_LEVEL m_featureLevel;
    ID3D11DeviceContext* m_deviceContext;

    IDXGIFactory* m_dxgiFactory;
    IDXGISwapChain* m_swapChain;
    DXGI_SWAP_CHAIN_DESC m_swapChainDescriptor;

#ifdef _DEBUG
    ID3D11Debug* m_debugInterface;
#endif
};
