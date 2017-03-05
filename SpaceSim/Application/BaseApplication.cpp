#include "Application/BaseApplication.h"
#include "Graphics/DeviceManager.h"
#include "Gameplay/GameObjectManager.h"
#include "Loader/MapLoader.h"
#include "Math/matrixmath.h"
#include "Core/Settings/SettingsManager.h"
#include "Core/Types/Types.h"
#include "Core/Resource/GameResource.h"
#include "Gameplay/LaserManager.h"
#include "Graphics/ShaderPack.h"
#include "Logging/FileLogger.h"
#include <wbemidl.h>
#include <oleauto.h>
#include <wbemidl.h>

#include <list>

#include "Graphics/CubeMapRenderer.h"
#include "Core/StringOperations/HashString.h"

#include "Gameplay/EntityManager.h"

#include "UI/BitmapFont.h"
#include "UI/TextBlock.h"

#include <brofiler.h>

class RenderInstance;

HashString exitGame("exit_game");

std::function<void(RAWINPUT*)> Application::m_inputDispatch;
Matrix44 Application::m_view;
Matrix44 Application::m_projection;
Logger Application::m_logger;

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

Text::TextBlockCache* cache;

//-----------------------------------------------------------------------------
//! @brief   Initialise the application
//! @remark
//-----------------------------------------------------------------------------
bool Application::initialise()
{
    m_logger.addLogger(new OutputDebugLog());
    FileLogger* file_logger = new FileLogger(m_paths.getLogPath());
    if (file_logger->is_open())
    {
        m_logger.addLogger(file_logger);
    }
    else
    {
        delete file_logger;
    }
    //m_logger.addLogger(new HttpDebugLog());
    
    //cache = new Text::TextBlockCache(1000, m_gameResource);

    m_gameResource = new GameResource(&m_logger, &m_paths, &m_settingsManager, &m_entityManager, &m_gameObjectManager,
        &m_laserManager, &m_uiManager, nullptr, &m_logger, &m_physicsManger);
    
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
    m_projection = math::createLeftHandedFOVPerspectiveMatrix(math::gmPI / 4.0f, (float)windowWidth / (float)windowHeight, 0.001f, 1500.0f);

    SettingsParser settings(&m_settingsManager);
    if (!settings.loadFile(m_paths.getSettingsPath() + "settings.cfg"))
    {
        MSG_TRACE_CHANNEL("BASEAPPLICATION", "Failed to load the settings file" )
        //terminate application if we fail to find the settings file
        returnValue &= false;
    }

    //m_uiManager.initialise();
    m_renderSystem.initialise(m_gameResource);
    //m_inputSystem.createController(Gamepad);
    m_inputSystem.initialise(m_paths.getSettingsPath() + "Input Maps\\input_mapping.xml", m_renderSystem.getWindowHandle());
    m_inputDispatch = &InputSystem::SetRawInput;

    m_laserManager.initialise(m_gameResource);
    MSG_TRACE_CHANNEL("REFACTOR", "SEND create Camera message to render system");

    Player* player = new Player(m_gameResource);
    //player->initialize(m_cameraSystem);
    m_gameObjectManager.addGameObject(player);


    //Test Code
    //m_cameraSystem.update(m_elapsedTime, m_time);
    //const Camera* cam = m_cameraSystem.getCamera("global");
    //m_view = cam->getCamera();

    //Text::BitmapFont bitmapFont;
    //bitmapFont.openFont("D:/SDK/Demo/SpaceSim/bin/FE/franklin.fnt.conv.fnt", m_gameResource);
    //cache->addFont("D:/SDK/Demo/SpaceSim/bin/FE/franklin.fnt.conv.fnt");
    //cache->addText("Hello World From Bitmap Font!", Vector4(0.f,0.f, 100.f, 500.f), Text::Align::left, bitmapFont.getFontInfo().m_fontNameHash, 48.0f, true);

    const ISetting<std::string>* mapFileName = m_settingsManager.getSetting<std::string>("SpaceStationMap");
    if (mapFileName)
    {
        returnValue &= m_map.loadMap(m_gameResource, mapFileName->getData());
    }

    MSG_TRACE_CHANNEL("BASEAPPLICATION", "Number of verts:  %d", Face::m_totalNumberOfVerts);
    MSG_TRACE_CHANNEL("BASEAPPLICATION", "Number of polies: %d", Face::m_totalNumberOfPolygons);

    //m_UpdateThread.m_cameraSystem = &m_cameraSystem;
    m_UpdateThread.m_entityManager = &m_entityManager;
    m_UpdateThread.m_gameObjectManager = &m_gameObjectManager;
    m_UpdateThread.m_laserManager = &m_laserManager;
    m_UpdateThread.m_settingsManager = &m_settingsManager;

    m_updateQueue = &m_messageQueue[0];
    m_renderQueue = &m_messageQueue[1];

    m_UpdateThread.SetMessageQueue(m_updateQueue);
    m_renderSystem.SetMessageQueue(m_renderQueue);

    m_UpdateThread.createThread(1024 * 1024, "UpdateThread"); //1MB stack space. this also kicks the first frame of simulation

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
            BROFILER_FRAME("NewSpaceSim Frame Marker");
            m_performanceTimer.update();
            m_elapsedTime = m_performanceTimer.getElapsedTime();
            m_time = m_performanceTimer.getTime();
            RenderInstanceTree renderList;
            Input input = m_inputSystem.getInput();
            //THis needs to happen in single threaded update
            {
                BROFILER_CATEGORY("SingleThreadedUpdate", Profiler::Color::Green);
                m_UpdateThread.LockCriticalSection();
                //const Camera* cam = m_cameraSystem.getCamera("global");
                //m_view = cam->getCamera();

                m_UpdateThread.setInput(input);

                renderList = m_UpdateThread.GetRenderInstanceList();
                m_UpdateThread.SetElapsedTime(m_elapsedTime, m_time);

                //Swap message queues here
                MessageSystem::MessageQueue* temp = m_renderQueue;
                m_renderQueue = m_updateQueue;
                m_renderQueue = temp;
                m_UpdateThread.SetMessageQueue(m_updateQueue);
                m_renderSystem.SetMessageQueue(m_renderQueue);

                m_UpdateThread.UnblockThread();
                m_UpdateThread.UnLockCriticalSection();
            }
            //Unblock simulation here
            m_inputSystem.update(m_elapsedTime, m_time);

            m_renderSystem.beginDraw(renderList);
            m_renderSystem.update(renderList, m_elapsedTime, m_time);
            m_renderSystem.endDraw();
            m_previousRenderInstanceListSize = renderList.size();

            InputActions::ActionType inputAction;
            InputSystem::getInputActionFromName(exitGame, inputAction);
            if (input.getInput(0) && input.getInput(0)->getActionValue(inputAction))
            {
                PostQuitMessage(0);
            }
        }
    }

    m_UpdateThread.stopThread();
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
    m_renderSystem.cleanup();
    m_gameResource->getGameObjectManager().cleanup();

    //Need to add a cleanup call to the cache
    //delete cache;
}

