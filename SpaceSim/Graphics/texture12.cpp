#include "Graphics/texture12.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"
#include "Graphics/D3DDebugHelperFunctions.h"
#include "Core/Paths.h"

#include "WICTextureLoader.h"
#include <DDSTextureLoader.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include "Graphics/D3D12/D3D12X.h"
#include <NewSpaceSim/packages/directxtk12_desktop_2017.2021.8.2.1/include/DirectXHelpers.h>
#include "D3D12/CommandQueue.h"
#include <optick.h>

Texture12::Texture12()
{
}

Texture12::~Texture12()
{
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool Texture12::loadTextureFromFile(DeviceManager& deviceManager, CommandQueueManager& commandQueueManager, const std::string& filename, size_t commandQeueuHandle, size_t commandListHandle, D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    auto& commandList = commandQueueManager.GetCommandQueue(commandQeueuHandle).GetCommandList(commandListHandle);
    return loadTextureFromFile(deviceManager, commandList, filename, handle);
}



bool Texture12::loadTextureFromFile(DeviceManager& deviceManager, CommandQueueManager& commandQueueManager, const std::string& filename) //shouldnt use this function
{
    MSG_TRACE_CHANNEL("TEXTURE12", "This cannot create a working texture");
    return loadTextureFromFile(deviceManager, commandQueueManager, filename, 0, 0, D3D12_CPU_DESCRIPTOR_HANDLE());
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool Texture12::loadTextureFromFile(DeviceManager& deviceManager, CommandList& commandList, const std::string& filename, D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    OPTICK_EVENT();

    ID3D12Device* device = deviceManager.GetDevice();
    HRESULT hr = S_OK;

    //Should convert through multibyte function this is dangerous and will fail on IME support
    std::string extension = extractExtensionFromFileName(filename);
    std::wstring wfilename;

    std::unique_ptr<uint8_t[]> textureData;
    std::vector<D3D12_SUBRESOURCE_DATA> subresources;

    convertToWideString(makeAbsolutePath(filename), wfilename);
    if (extension == "dds")
    {
        hr = DirectX::LoadDDSTextureFromFile(device, wfilename.c_str(), &m_texture, textureData, subresources);

        //hr = DirectX::CreateDDSTextureFromFile(device, deviceManager.getDeferredDeviceContext(), wfilename.c_str(), 0, &m_textureShaderResourceView);
        //D3DDebugHelperFunctions::SetDebugChildName(m_textureShaderResourceView, FormatString("SRV for Texture %s", filename.c_str()));
    }
    else
    {
        subresources.push_back(D3D12_SUBRESOURCE_DATA());
        hr = DirectX::LoadWICTextureFromFile(device, wfilename.c_str(), &m_texture, textureData, subresources[0]);
        //hr = DirectX::CreateWICTextureFromFile(device, deviceManager.getDeferredDeviceContext(), wfilename.c_str(), 0, &m_textureShaderResourceView, 0);
        //D3DDebugHelperFunctions::SetDebugChildName(m_textureShaderResourceView, FormatString("RTV for Texture %s", filename.c_str()));
    }

    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("Texture12", "Failed to load texture (%s) for reason: %d, %s", filename.c_str(), hr, getLastErrorMessage(hr));
        MSG_TRACE_CHANNEL("Texture12", "Fail reason: %s", getLastErrorMessage(deviceManager.GetDevice()->GetDeviceRemovedReason()));
        return false;
    }


    const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture, 0, static_cast<UINT>(subresources.size()));

    // Create the GPU upload buffer.
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

    auto desc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

    ID3D12Resource* uploadRes;
    hr = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadRes));

    //have to pass commandlist here

    UpdateSubresources(commandList.m_list, m_texture, uploadRes, 0, 0, static_cast<UINT>(subresources.size()), subresources.data());

    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_texture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    commandList.m_list->ResourceBarrier(1, &barrier);

    //Need to find a way to get this here
    DirectX::CreateShaderResourceView(device, m_texture, handle);
    m_handle = handle;
    m_isValid = true;
    //Create resource view here, CPU handle

    //m_gpuAddress = m_texture->GetGPUVirtualAddress();

#ifdef _DEBUG
    std::wstringstream str;
    str << L"Texture:" << wfilename;
    m_texture->SetName(str.str().c_str());
#endif


    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("TEXTURE", "Failed to load %s %d", filename.c_str(), hr);
        return false;
    }

    ////Create the sampler state for the texture
    //D3D11_SAMPLER_DESC textureSamplerDescription;
    //ZeroMemory(&textureSamplerDescription, sizeof(D3D11_SAMPLER_DESC));
    //textureSamplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
    //textureSamplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    //textureSamplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    //textureSamplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    //if (FAILED(device->CreateSamplerState(&textureSamplerDescription, &m_textureSamplerState) ) )
    //{
    //    MSG_TRACE_CHANNEL("ERROR", "Failed to create Sampler State" )
    //}
    //
    //D3DDebugHelperFunctions::SetDebugChildName(m_textureSamplerState, FormatString("Sampler for Texture %s", filename.c_str()));

    return true;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Texture12::cleanup()
{
    //if (!m_renderTargetViews.empty())
    //{
    //for (ID3D11RenderTargetView* rt: m_renderTargetViews)
    //{
    //rt->Release();
    //rt = nullptr;
    //}
    //}

    if (m_texture && m_isValid)
    {
        m_texture->Release();
        m_texture = nullptr;
        m_isValid = false;
    }
}