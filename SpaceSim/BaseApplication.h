#pragma once

#include <WinSock2.h>

#include "CameraManager.h"
#include "RenderSystem.h"
#include "GameWindow.h"
#include "SettingsParser.h"
#include "SettingsManager.h"
#include "GameObjectManager.h"
#include "particlesystemmanager.h"
#include "LaserManager.h"
#include "LightManager.h"
#include "GameResource.h"
#include "InputSystem.h"
#include "MapLoader.h"
#include "Timer.h"
#include "EffectCache.h"
#include "ShaderCache.h"
#include "Paths.h"
#include "UIManger.h"
#include "EntityManager.h"

#include "MeshComponentData.h"

#include <windows.h>
#include "Memory.h"

#include "Http Server/httpserver.h"
#include "Logging/Logger.h"


//-----------------------------------------------------------------------------
//! @brief   Base class for an application object
//! @remark  Defines the wndProc used in the Window class it contains
//-----------------------------------------------------------------------------
class Application
{
public:
    Application();
    ~Application() 
    {
        if (m_gameResource != nullptr)
        {
            delete m_gameResource;
            m_gameResource = nullptr;
        }
    }

    void cleanup();
    bool initialise();

    void mainGameLoop();

    static LRESULT CALLBACK messageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    static std::function<void(RAWINPUT*)> m_inputDispatch;

    const GameResource* getResource() const { return m_gameResource; }    
private:
    EntityManager      m_entityManager;
    RenderSystem       m_renderSystem;
    CameraManager      m_cameraSystem;
    SettingsManager    m_settingsManager;
    GameObjectManager  m_gameObjectManager;
    ParticleSystemManager m_pfxManager;
    LightManager m_lightManager;
    LaserManager    m_laserManager;
    ShaderCache m_shaderCache;
    EffectCache m_effectCache;
    UIManger m_uiManager;
    HTTPServer m_httpServer;
    GameResource*      m_gameResource;
    bool               m_controllerConnected;
    bool               m_useController;
    PerformanceTimer   m_performanceTimer;
    float              m_elapsedTime;
    double             m_time;
    InputSystem        m_inputSystem;
    MapLoader                m_map;
    Paths              m_paths;

    size_t m_previousRenderInstanceListSize;


    //ModelComponentManger m_modelManager;
public:
    static Matrix44 m_view;
    static Matrix44 m_projection;

    static Logger m_logger;
};
