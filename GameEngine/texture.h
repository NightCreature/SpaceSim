#ifndef TEXTURE_H
#define TEXTURE_H
#include "..\SpaceSim\StringHelperFunctions.h"
#include <d3d11.h>
#include <string>
#include <vector>
#include <windows.h>
#include "Memory.h"

class DeviceManager;

class Texture
{
public:
	Texture();
	~Texture();

    void cleanup();
	bool loadTextureFromFile(const DeviceManager& deviceManager, std::string filename);
    void createFromShaderResourceView(ID3D11ShaderResourceView* textureShaderResourceView, ID3D11SamplerState* textureSamplerState) { m_textureShaderResourceView = textureShaderResourceView; m_textureSamplerState = textureSamplerState; }
    ID3D11ShaderResourceView* getShaderResourceView() const { return m_textureShaderResourceView; }
    ID3D11SamplerState* getSamplerState() const { return m_textureSamplerState; }
    ID3D11RenderTargetView* getRenderTargetView(size_t index) { return index < 6 ? m_renderTargetViews[index] : nullptr; }
    ID3D11Texture2D* getTextureResource() { return m_texture; }
    void createRenderTarget( ID3D11Texture2D* textureResource, ID3D11RenderTargetView* rtView[6], ID3D11ShaderResourceView* srView )
    {
        m_texture = textureResource;
        for (size_t counter = 0; counter < 6; ++counter)
        {
            m_renderTargetViews[counter] = rtView[counter];
        }
        m_textureShaderResourceView = srView;
    }
    HASH_ELEMENT_DEFINITION
protected:
private:
    ID3D11Texture2D* m_texture;
    ID3D11RenderTargetView*  m_renderTargetViews[6];
    ID3D11ShaderResourceView* m_textureShaderResourceView;
    ID3D11SamplerState*       m_textureSamplerState;
};
#endif