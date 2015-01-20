#include "BaseApplication.h"
#include "DeviceManager.h"
#include "GameObjectManager.h"
#include "MapLoader.h"
#include "matrixmath.h"
#include "SettingsManager.h"
#include "Types.h"
#include "GameResource.h"
#include "LaserManager.h"
#include "ShaderPack.h"
#include <wbemidl.h>
#include <oleauto.h>
#include <wbemidl.h>

#include <list>

#include "CubeMapRenderer.h"

#include "ittnotify.h"

class RenderInstance;

std::function<void(RAWINPUT*)> Application::m_inputDispatch;
Matrix44 Application::m_view;
Matrix44 Application::m_projection;
//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
Application::Application():
m_gameResource(nullptr),
m_previousRenderInstanceListSize(1)
{
    m_controllerConnected = false;
    m_useController = false;
    //m_controller = 0;
}

//-----------------------------------------------------------------------------
//! @brief   Initialise the application
//! @remark
//-----------------------------------------------------------------------------
bool Application::initialise()
{
    m_gameResource = new GameResource(&m_cameraSystem, &m_renderSystem.getDeviceMananger(), &m_settingsManager, &m_renderSystem.getTextureManager(), &m_gameObjectManager, &m_renderSystem.getModelManger(),
                                      &m_pfxManager, &m_lightManager, &m_laserManager, &m_shaderCache, &m_effectCache, &m_paths);
    bool returnValue = true;

    int windowWidth = 1280;
    int windowHeight = 720;
    const ISetting<int>* widthSetting = m_settingsManager.getSetting<int>("WindowWidth");
    if (widthSetting)
    {
        windowWidth = widthSetting->getData();     
    }
    const ISetting<int>* heightSetting = m_settingsManager.getSetting<int>("WindowHeight");
    if (heightSetting)
    {
        windowHeight = heightSetting->getData();     
    }
    m_projection = math::createLeftHandedFOVPerspectiveMatrix(math::gmPI / 4.0f, (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f);

    SettingsParser settings(&m_settingsManager);
    if (!settings.loadFile(m_paths.getSettingsPath() + "settings.cfg"))
    {
        MSG_TRACE_CHANNEL("BASEAPPLICATION", "Failed to load the settings file" )
        //terminate application if we fail to find the settings file
        returnValue &= false;
    }

    m_renderSystem.initialise(m_gameResource);
    //m_inputSystem.createController(Gamepad);
    m_inputSystem.initialise(m_paths.getSettingsPath() + "Input Maps\\input_mapping.xml");
    m_inputDispatch = &InputSystem::SetRawInput;
    ShaderPack shaderPack(m_gameResource);
    shaderPack.loadShaderPack("shader_pack.xml");
    m_laserManager.initialise(m_gameResource);
    returnValue &= m_cameraSystem.createCamera(*m_gameResource, "global", Vector3(0.0f, 0.0f, 200.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3::yAxis());
    returnValue &= m_cameraSystem.createCamera(*m_gameResource, "player_camera", Vector3(0.0f, 0.0f, 200.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3::yAxis());
    Player* player = new Player(m_gameResource);
    player->initialize(m_cameraSystem);
    m_gameObjectManager.addGameObject(player);

    const ISetting<std::string>* mapFileName = m_settingsManager.getSetting<std::string>("SpaceStationMap");
    if (mapFileName)
    {
        returnValue &= m_map.loadMap(m_gameResource, mapFileName->getData());
    }

    MSG_TRACE_CHANNEL("BASEAPPLICATION", "Number of verts:  %d", Face::m_totalNumberOfVerts )
    MSG_TRACE_CHANNEL("BASEAPPLICATION", "Number of polies: %d", Face::m_totalNumberOfPolygons )

    return returnValue;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Application::mainGameLoop()
{
    MSG message;
    message.message = WM_NULL;
    PeekMessage(&message, 0, 0, 0, PM_NOREMOVE);

    while ( WM_QUIT != message.message )
    {
        message.message = WM_NULL;
        bool gotMessage = ( PeekMessage(&message, 0, 0, 0, PM_NOREMOVE) != 0 );
        if (gotMessage)
        {
            PeekMessage(&message, 0, 0, 0, PM_REMOVE );
            TranslateMessage( &message );
            DispatchMessage( &message );
        }
        else
        {
            __itt_domain *my_itt_frame = __itt_domain_create("Application::mainGameLoop");
            my_itt_frame->flags = 1; //enable it
            __itt_frame_begin_v3(my_itt_frame, NULL);

            m_performanceTimer.update();
            m_elapsedTime = m_performanceTimer.getElapsedTime();
            m_time = m_performanceTimer.getTime();

            m_inputSystem.update(m_elapsedTime, m_time);
            Input input = m_inputSystem.getInput();
            m_cameraSystem.update(m_elapsedTime, m_time, input);
            const Camera* cam = m_cameraSystem.getCamera("global");
            m_view = cam->getCamera();
            
            RenderInstanceTree renderList;
            renderList.reserve(m_previousRenderInstanceListSize); //Upfront reserve as much space as the last frame used, it should at max from once or twice a frame this way, ignoring the first one
            m_gameObjectManager.update(renderList, m_elapsedTime, input);
            m_gameResource->getLaserManager()->update(renderList, m_elapsedTime, m_renderSystem.getDeviceMananger());

            m_renderSystem.beginDraw(renderList, m_gameResource);
            m_renderSystem.update(m_gameResource, renderList, m_elapsedTime, m_time);
            m_renderSystem.endDraw();
            m_previousRenderInstanceListSize = renderList.size();

            //if (input.getInput(0)->getActionValue(InputActions::eAButton))
            //{
            //    m_renderSystem.flipWireFrameMod();
            //}
            //if (input.getInput(1)->getActionValue(m_inputSystem.getInputActionFromName("exit_game")))
            //{
            //    PostQuitMessage(0);
            //}

            __itt_frame_end_v3(my_itt_frame, NULL);
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief  Pass win32 messages on to the window
//-----------------------------------------------------------------------------
LRESULT CALLBACK Application::messageHandler( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if (message == WM_DESTROY)
    {
        PostQuitMessage(0);
    }
    else if (message == WM_INPUT)
    {
        char buffer[sizeof(RAWINPUT)] = {};
        UINT size = sizeof(RAWINPUT);
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));

        // extract keyboard raw input data
        RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);
        m_inputDispatch(raw);
    }

    //If the window doesn't exist yet pass on all the messages to the default win32 message handler
    return DefWindowProc (hwnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
//! @brief  Cleanup the instance and window
//-----------------------------------------------------------------------------
void Application::cleanup()
{
    m_gameResource->getDeviceManager()->clearDeviceState();
    m_gameResource->getSettingsManager()->cleanup();
    m_renderSystem.cleanup();
    m_gameResource->getTextureManager()->cleanup();
    m_gameResource->getDeviceManager()->cleanup();
    m_gameResource->getModelManager()->cleanup();
    m_gameResource->getGameObjectManager()->cleanup();
}

