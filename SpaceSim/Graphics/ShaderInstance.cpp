#include "Graphics/ShaderInstance.h"

#include "Graphics/DeviceManager.h"
#include "Graphics/D3DDebugHelperFunctions.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
ID3D11Buffer* CreateD3DBuffer(size_t bufferSize, DeviceManager &deviceManager, const std::string &name)
{
    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
    constantBufferDesc.ByteWidth = static_cast<UINT>(bufferSize);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ID3D11Buffer* buffer = nullptr;

    HRESULT hr = deviceManager.getDevice()->CreateBuffer(&constantBufferDesc, 0, &buffer);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("EFFECT_ERROR", "failed to create the material buffer for this effect: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
        return nullptr;
    }
    D3DDebugHelperFunctions::SetDebugChildName(buffer, FormatString("%s Constant Buffer with size %u", name.c_str(), bufferSize / sizeof(float)));

    return buffer;
}


///-----------------------------------------------------------------------------
///! @brief bufferSize is in bytes, so should be the amount of floats we need for this buffer
///! @remark
///-----------------------------------------------------------------------------
void ShaderInstance::AddPsConstantBuffer(size_t bufferSize, DeviceManager& deviceManager, const std::string& name)
{
    ID3D11Buffer* buffer = CreateD3DBuffer(bufferSize, deviceManager, name);

    if (buffer != nullptr)
    {
        m_psConstantBuffers.push_back(buffer);
    }
}

///-----------------------------------------------------------------------------
///! @brief bufferSize is in bytes, so should be the amount of floats we need for this buffer
///! @remark
///-----------------------------------------------------------------------------
void ShaderInstance::AddVsConstantBuffer(size_t bufferSize, DeviceManager& deviceManager, const std::string& name)
{
    ID3D11Buffer* buffer = CreateD3DBuffer(bufferSize, deviceManager, name);
    
    if (buffer != nullptr)
    {
        m_vsConstantBuffers.push_back(buffer);
    }
}