#pragma once
#include "Core/StringOperations/StringHelperFunctions.h"
#include <d3d12.h>
#include <string>
#include <vector>
#include <windows.h>
#include "Memory.h"

class DeviceManager;

class Texture12
{
public:
	Texture12();
	~Texture12();

    void cleanup();
    bool loadTextureFromFile(DeviceManager& deviceManager, const std::string& filename);
    bool loadTextureFromFile(DeviceManager& deviceManager, const std::string& filename, size_t commandQeueuHandle, size_t commandListHandle, D3D12_CPU_DESCRIPTOR_HANDLE handle);
    //void createFromShaderResourceView(ID3D11ShaderResourceView* textureShaderResourceView, ID3D11SamplerState* textureSamplerState) { m_textureShaderResourceView = textureShaderResourceView; m_textureSamplerState = textureSamplerState; }
    //ID3D11ShaderResourceView* getShaderResourceView() const { return m_textureShaderResourceView; }
    //ID3D11SamplerState* getSamplerState() const { return m_textureSamplerState; }
    //ID3D11RenderTargetView* getRenderTargetView(size_t index) { return index < 6 ? m_renderTargetViews[index] : nullptr; }
    ID3D12Resource* getTextureResource() { return m_texture; }
    //void createRenderTarget( ID3D11Texture2D* textureResource, ID3D11RenderTargetView* rtView[6], ID3D11ShaderResourceView* srView )
    //{
    //    m_texture = textureResource;
    //    for (size_t counter = 0; counter < 6; ++counter)
    //    {
    //        m_renderTargetViews[counter] = rtView[counter];
    //    }
    //    m_textureShaderResourceView = srView;
    //}
    HASH_ELEMENT_DEFINITION(Texture12)
protected:
private:
    ID3D12Resource* m_texture;
};