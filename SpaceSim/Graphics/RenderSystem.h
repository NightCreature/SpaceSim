#pragma once

#include "Application/GameWindow.h"
#include "Input/Input.h"
#include "Graphics/OrientationAxis.h"
#include "Core/Types/Types.h"
#include <d3d11.h>
#include <dxgi.h>
#include <list>
#include <directxmath.h>
#include "Graphics/DeviceManager.h"
#include "Graphics/texturemanager.h"
#include "Graphics/modelmanager.h"
#include "Graphics/CubeMapRenderer.h"
#include "Graphics/ShadowMapRenderer.h"

#ifdef _DEBUG
#include <d3d11_1.h>
#include <atlbase.h>
#endif

class RenderInstance;

struct CubeRendererInitialiseData
{
    HASH_ELEMENT_DEFINITION;
    void deserialise(const tinyxml2::XMLElement* node);
    std::string m_texutureResourceName;
    Vector3 m_position;
    bool m_dynamic;
    bool m_hasBeenRenderedTo;
};

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
class RenderSystem 
{
public:
	RenderSystem();
    ~RenderSystem();

    void initialise(Resource* resource);
    
    void cleanup();

    void beginDraw(RenderInstanceTree& renderInstances, Resource* resource);

    void CheckVisibility(RenderInstanceTree& renderInstances);

    void update(Resource* resource, RenderInstanceTree& renderInstances, float elapsedTime, double time);

    void endDraw( Resource* resource );

    HWND getWindowHandle() const { return m_window.getWindowHandle(); }
    const IDXGIFactory* getDXGIFactory() const { return m_dxgiFactory; }

    void flipWireFrameMod() { m_wireFrame = !m_wireFrame; }

    DeviceManager& getDeviceMananger() { return m_deviceManager; }
    TextureManager& getTextureManager() { return m_textureManager; }
    ModelManager& getModelManger() { return m_modelManger; }

    CubeMapRenderer* getCubeMapRenderer() { return m_cubeMapRenderer; }
protected:
private:
    bool createSwapChain(ID3D11Device* device, int windowWidth, int windowHeight);
    void patchUpDXGIFactory(ID3D11Device* device);
    void setupSwapChainForRendering( ID3D11Device* device, ID3D11DeviceContext* deviceContext, int windowWidth, int windowHeight );
    void initialiseCubemapRendererAndResources( GameResourceHelper &resourceHelper );

    DeviceManager m_deviceManager;
    TextureManager m_textureManager;
    ModelManager m_modelManger;
    GameWindow m_window;
    std::string m_appName;
	std::string m_windowName;

    IDXGIFactory* m_dxgiFactory;
    IDXGISwapChain* m_swapChain;
    DXGI_SWAP_CHAIN_DESC m_swapChainDescriptor;

    ID3D11Texture2D* m_backBuffer;
    ID3D11Texture2D* m_depthStencilBuffer;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11DepthStencilView* m_depthStencilView;

    ID3D11RasterizerState* m_rasteriserState;
    ID3D11RasterizerState* m_rasteriserWireFrameModeState;
    ID3D11BlendState* m_blendState;
    ID3D11BlendState* m_alphaBlendState;
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11Buffer* m_lightConstantBuffer;
    bool m_wireFrame;

    size_t m_numberOfInstancesRenderingThisFrame;
    size_t m_totalNumberOfInstancesRendered;
    size_t m_averageNumberOfInstancesRenderingPerFrame;
    size_t m_totalNumberOfRenderedFrames;
#ifdef _DEBUG
    OrientationAxis* m_debugAxis;
    CComPtr<ID3DUserDefinedAnnotation> pPerf;
#endif

    std::vector<CubeRendererInitialiseData> m_cubeSettings;
    CubeMapRenderer* m_cubeMapRenderer;
    ShadowMapRenderer* m_shadowMapRenderer;
    ID3D11SamplerState* m_samplerState;


    RenderInstanceTree visibleInstances;
};