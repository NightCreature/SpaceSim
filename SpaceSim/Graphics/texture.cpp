#include "Graphics/texture.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/D3DDebugHelperFunctions.h"
#include "Core/Paths.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

#include <assert.h>
#include <iostream>
#include <sstream>


HASH_ELEMENT_IMPLEMENTATION(Texture)
Texture::Texture()
{
    m_textureShaderResourceView = nullptr;
    m_textureSamplerState = nullptr;
}

Texture::~Texture()
{
}

bool Texture::loadTextureFromFile(const DeviceManager& deviceManager, const std::string& filename)
{
    ID3D11Device* device = deviceManager.getDevice();
    HRESULT hr = S_OK;

	//Should convert through multibyte function this is dangerous and will fail on IME support
    std::string extension = extractExtensionFromFileName(filename);
	std::wstring wfilename;

	convertToWideString(makeAbsolutePath(filename), wfilename);
    if (extension == "dds")
    {
        hr = DirectX::CreateDDSTextureFromFile(device, wfilename.c_str(), 0, &m_textureShaderResourceView);
        //D3DDebugHelperFunctions::SetDebugChildName(m_textureShaderResourceView, FormatString("SRV for Texture %s", filename.c_str()));
    }
    else
    {

        hr = DirectX::CreateWICTextureFromFile(device, deviceManager.getDeviceContext(), wfilename.c_str(), 0, &m_textureShaderResourceView, 0);
        //D3DDebugHelperFunctions::SetDebugChildName(m_textureShaderResourceView, FormatString("RTV for Texture %s", filename.c_str()));
    }

    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("TEXTURE", "Failed to load %s %d", filename.c_str(), hr);
        return false;
    }

    //Create the sampler state for the texture
    D3D11_SAMPLER_DESC textureSamplerDescription;
    ZeroMemory(&textureSamplerDescription, sizeof(D3D11_SAMPLER_DESC));
    textureSamplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
    textureSamplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    textureSamplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    textureSamplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    if (FAILED(device->CreateSamplerState(&textureSamplerDescription, &m_textureSamplerState) ) )
    {
        MSG_TRACE_CHANNEL("ERROR", "Failed to create Sampler State" )
    }
    
    //D3DDebugHelperFunctions::SetDebugChildName(m_textureSamplerState, FormatString("Sampler for Texture %s", filename.c_str()));

	return true;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Texture::cleanup()
{
    //if (!m_renderTargetViews.empty())
    //{
    //for (ID3D11RenderTargetView* rt: m_renderTargetViews)
    //{
    //rt->Release();
    //rt = nullptr;
    //}
    //}
    if (m_textureShaderResourceView)
    {
        m_textureShaderResourceView->Release();
        m_textureShaderResourceView = nullptr;
    }
    if (m_textureSamplerState)
    {
        m_textureSamplerState->Release();
        m_textureSamplerState = nullptr;
    }

    for (size_t counter = 0; counter < 6; ++counter)
    {
        if (m_renderTargetViews[counter] != nullptr)
        {
            m_renderTargetViews[counter]->Release();
            m_renderTargetViews[counter] = nullptr;
        }
    }

    if (m_texture)
    {
        m_texture->Release();
        m_texture = nullptr;
    }
}