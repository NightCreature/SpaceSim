#pragma once

#include "Application/GameWindow.h"
#include "Core/MessageSystem/MessageObserver.h"
#include "Core/Resource/RenderResource.h"
#include "Core/Thread/Job.h"
#include "Core/Thread/JobSystem.h"
#include "Core/Thread/Timer.h"
#include "Core/Types/Types.h"
#include "Gameplay/particleemitter.h"
#include "Gameplay/particlesystemmanager.h" //This should move away from the gameplay directory
#include "Graphics/CameraManager.h"
#include "Graphics/CubeMapRenderer.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"
#include "Graphics/EffectCache.h"
#include "Graphics/LightManager.h"
#include "Graphics/OrientationAxis.h"
#include "Graphics/ShaderCache.h"
#include "Graphics/ShadowMapRenderer.h"
#include "Graphics/modelmanager.h"
#include "Graphics/texturemanager.h"
#include "Input/Input.h"
#include "Loader/ResourceLoader.h"

#include <d3d11.h>
#include <directxmath.h>
#include <dxgi.h>
#include <list>

#ifdef _DEBUG
#include <d3d11_1.h>
#endif
#include "D3D12/DescriptorHeapManager.h"
#include "D3D12/D3D12X.h"
#include "D3D12/CommandQueue.h"
#include "Graphics/RenderInterface.h"
#include "Graphics/D3D12/ConstantBufferManager.h"

class RenderInstance;
namespace MessageSystem
{
class MessageQueue;
class Message;
}

struct CubeRendererInitialiseData
{
    HASH_ELEMENT_DEFINITION(CubeRendererInitialiseData);
    void deserialise(const tinyxml2::XMLElement* node);
    std::string m_texutureResourceName;
    Vector3 m_position;
    bool m_dynamic;
    bool m_hasBeenRenderedTo;
};

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
class RenderSystem 
{
public:
	RenderSystem();
    ~RenderSystem();

    void initialise(Resource* resource);

    void cleanup();
    void beginDraw();
    void CheckVisibility(RenderInstanceTree& renderInstances);
    void update(float elapsedTime, double time);

    void endDraw();

    HWND getWindowHandle() const { return m_window.getWindowHandle(); }

    void flipWireFrameMod() { m_wireFrame = !m_wireFrame; }

    DeviceManager& getDeviceMananger() { return m_deviceManager; }
    TextureManager& getTextureManager() { return m_textureManager; }
    ModelManager& getModelManger() { return m_modelManger; }

    CubeMapRenderer* getCubeMapRenderer() { return m_cubeMapRenderer; }

    void setInput(Input input) { m_input = input; }

    size_t GetSceneTransformBufferIndex() const { return m_sceneTransformBuffer.GetHeapIndex(); }
    size_t GetCameraBufferIndex() const { return m_cameraBuffer.GetHeapIndex(); }
    
    Resource* GetResource() { return m_renderResource; }

    //Fix this
    static Matrix44 m_view;
    static Matrix44 m_inverseView;
    static Matrix44 m_projection;
protected:
private:
    void setupSwapChainForRendering( ID3D11Device* device, ID3D11DeviceContext* deviceContext, int windowWidth, int windowHeight );
    void CreatePipelineStates(ID3D11Device* device);
    void initialiseCubemapRendererAndResources( Resource* resource );
    
    Matrix44 m_CullingProjectionMatrix;


    RenderResource* m_renderResource;

    CameraManager      m_cameraSystem;
    CommandQueueManager m_commandQueueManager;
    DeviceManagerD3D11 m_deviceManager11;
    DeviceManager m_deviceManager;
    DescriptorHeapManager m_heapManager;
    TextureManager m_textureManager;
    ModelManager m_modelManger;
    LightManager m_lightManager;
    ParticleSystem::ParticleSystemManager m_pfxManager;
    ShaderCache m_shaderCache;
    EffectCache m_effectCache;
    GameWindow m_window;
    ResourceLoader m_resourceLoader;
    JobQueue m_jobQueue;
    PerFrameDataStorage m_perFrameDataStorage;
    std::string m_appName;
    std::string m_windowName;

    IDXGISwapChain* m_swapChain; //Non owning pointer
    D3D12_VIEWPORT m_viewPort;
    CD3DX12_RECT m_scissorRect;

    //This needs to change
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
    ID3D11Buffer* m_shadowConstantBuffer;
    ConstantBuffer m_sceneTransformBuffer;
    ConstantBuffer m_cameraBuffer;

#ifdef PROFILING
    ID3D11Query* m_beginDrawQuery;
    ID3D11Query* m_endDrawQuery;
    ID3D11Query* m_cubemapBeginDrawQuery;
    ID3D11Query* m_cubemapEndDrawQuery;
    ID3D11Query* m_shadoBeginDrawQuery;
    ID3D11Query* m_shadoEndDrawQuery;
    ID3D11Query* m_mainBeginDrawQuery;
    ID3D11Query* m_mainEndDrawQuery;
    ID3D11Query* m_timeDisjointQuery;

    ID3D11Query* m_pipeLineStatistics;
#endif
    bool m_wireFrame;

    size_t m_numberOfInstancesRenderingThisFrame;
    size_t m_totalNumberOfInstancesRendered;
    size_t m_averageNumberOfInstancesRenderingPerFrame;
    size_t m_totalNumberOfRenderedFrames;
#ifdef _DEBUG
    OrientationAxis* m_debugAxis;
    ID3DUserDefinedAnnotation* pPerf;
#endif

    std::vector<CubeRendererInitialiseData> m_cubeSettings;
    CubeMapRenderer* m_cubeMapRenderer;
    ShadowMapRenderer* m_shadowMapRenderer;
    ID3D11SamplerState* m_samplerState; //Fix this

    
    RenderInstanceTree visibleInstances;

    MessageSystem::MessageObserver m_messageObservers;


    //TEMP HACK
    RenderInstanceTree m_renderInstances;

    //No more need for these to be static
    Input m_input;

    size_t m_numberOfInstancePerFrame;


    //Test shit
    ParticleSystem::ParticleEmitterComponentBased m_emmiter;
    ///-----------------------------------------------------------------------------
    ///! @brief 
    ///! @remark
    ///-----------------------------------------------------------------------------
    void CreateMainCommandList();

    size_t m_tempCommandList;
    size_t m_renderCommandQueueHandle = 0;
    size_t m_currentCommandListIndex = 0;
    size_t m_backUpCommandListIndex = 0;
};