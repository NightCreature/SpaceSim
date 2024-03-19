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
//#include <wbemidl.h>
//#include <oleauto.h>
//#include <wbemidl.h>

#include <list>

#include "Graphics/CubeMapRenderer.h"
#include "Core/StringOperations/HashString.h"

#include "Gameplay/EntityManager.h"

#include "UI/BitmapFont.h"
#include "UI/TextBlock.h"

#include "Core/Profiler/ProfilerMacros.h" 
#include "Core/Profiler/Profiler.h"

#ifdef DEBUG
#include "vld.h"
#endif

#include "../NodeGraph/GraphDefinitionReader.h"
#include "../Core/FileSystem/FileSystem.h"
#include "../Core/FileSystem/Platform/Win/MountPointWin.h"
#include "Generators/MapGenerator.h"

#include "Core/ConstructionFactory.h"

#include<optick.h>
#include "Core/Serialization/Archive.h"
#include "Core/Serialization/ISerializable.h"

class RenderInstance;

std::function<void(RAWINPUT*)> Application::m_inputDispatch;
Logger Application::m_logger;
constexpr size_t numOfJobsToSpawn = 16;

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
Application::Application():
m_jobSystem(numOfJobsToSpawn), //Spawn the job queue with 8 threads
m_gameResource(nullptr),
m_fileSystem(m_paths),
m_previousRenderInstanceListSize(1)
{
    m_controllerConnected = false;
    m_useController = false;
    //m_controller = 0;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool Application::initialise()
{
    OPTICK_EVENT();

    m_logger.addLogger(new OutputDebugLog());
    FileLogger* file_logger = new FileLogger(m_paths.getLogPathStr());
    if (file_logger->is_open())
    {
        m_logger.addLogger(file_logger);
    }
    else
    {
        delete file_logger;
    }

    m_gameResource = new GameResource(&m_logger, &m_messageQueues, &m_paths, &m_performanceTimer, &m_settingsManager, &m_fileSystem, &m_entityManager, &m_gameObjectManager,
        &m_laserManager, &m_uiManager, nullptr, &m_logger, m_jobSystem.GetJobQueuePtr());
    
    m_jobSystem.SetGameResource(m_gameResource);

    bool returnValue = true;


    //int windowWidth = 1280;
    //int windowHeight = 720;
    //const ISetting<int>* widthSetting = m_settingsManager.getSetting<int>("WindowWidth");
    //if (widthSetting)
    //{
    //    windowWidth = widthSetting->getData();     
    //}
    //const ISetting<int>* heightSetting = m_settingsManager.getSetting<int>("WindowHeight");
    //if (heightSetting)
    //{
    //    windowHeight = heightSetting->getData();     
    //}

    SettingsParser settings(&m_settingsManager);
    auto settingsPath = m_paths.getSettingsPath() / "settings.cfg";
    if (!settings.loadFile(settingsPath.string()))
    {
        MSG_TRACE_CHANNEL("BASEAPPLICATION", "Failed to load the settings file" )
        //terminate application if we fail to find the settings file
        returnValue &= false;
    }

    //Settings are loaded now stream them back out to an archive file to see if we can read that back in
    m_settingsManager.SaveSettings(m_paths.getSettingsPath() / "settings.archive");
    //m_settingsManager.LoadSettings(m_paths.getSettingsPath() / "settings.archive");

    //m_uiManager.initialise();
    m_renderSystem.initialise(m_gameResource);
    m_jobSystem.SetRenderResource(m_renderSystem.GetResource());
    //m_inputSystem.createController(Gamepad);
    auto inputMapPath = m_paths.getSettingsPath() / "Input Maps\\input_mapping.xml";
    m_inputSystem.initialise(inputMapPath.string(), m_renderSystem.getWindowHandle());
    m_inputDispatch = &InputSystem::SetRawInput;

    m_physicsManger.Initialise(m_gameResource);

    m_laserManager.initialise(m_gameResource);
    MSG_TRACE_CHANNEL("REFACTOR", "SEND create Camera message to render system");

    //TODO This has to go
    Player* player = new Player(m_gameResource);
    player->initialize();
    m_gameObjectManager.addGameObject(player);


    auto graphPath = m_paths.getPath() / "graphs";
    auto graphFilename = graphPath / "graphDefinitions.xml";
    NodeGraph::GraphReader graphReader;
    graphReader.readGraphDefinition(graphFilename);

    //Test Code
    //m_cameraSystem.update(m_elapsedTime, m_time);
    //const Camera* cam = m_cameraSystem.getCamera("global");
    //m_view = cam->getCamera();

    //Text::BitmapFont bitmapFont;
    //bitmapFont.openFont("D:/SDK/Demo/SpaceSim/bin/FE/franklin.fnt.conv.fnt", m_gameResource);
    //cache->addFont("D:/SDK/Demo/SpaceSim/bin/FE/franklin.fnt.conv.fnt");
    //cache->addText("Hello World From Bitmap Font!", Vector4(0.f,0.f, 100.f, 500.f), Text::Align::left, bitmapFont.getFontInfo().m_fontNameHash, 48.0f, true);

    ///!!This needs to move
    const ISetting<std::string>* mapFileName = m_settingsManager.getSetting<std::string>("SpaceStationMap");
    if (mapFileName)
    {
        returnValue &= m_map.loadMap(m_gameResource, mapFileName->getData());
    }
    ///!!

    MSG_TRACE_CHANNEL("BASEAPPLICATION", "Number of verts:  %d", Face::m_totalNumberOfVerts);
    MSG_TRACE_CHANNEL("BASEAPPLICATION", "Number of polies: %d", Face::m_totalNumberOfPolygons);

    //m_UpdateThread.m_cameraSystem = &m_cameraSystem;
    m_UpdateThread.m_entityManager = &m_entityManager;
    m_UpdateThread.m_gameObjectManager = &m_gameObjectManager;
    m_UpdateThread.m_laserManager = &m_laserManager;
    m_UpdateThread.m_physicsManager = &m_physicsManger;

    m_UpdateThread.Initialise(m_gameResource);

    m_UpdateThread.createThread(1024 * 1024, "UpdateThread"); //1MB stack space. this also kicks the first frame of simulation

    return returnValue;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
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
            OPTICK_EVENT("TranslateMessage");
            PeekMessage(&message, 0, 0, 0, PM_REMOVE );
            TranslateMessage( &message );
            DispatchMessage( &message );
        }
        else
        {
            PROFILE_FRAME("NewSpaceSim Frame Marker");
            OPTICK_FRAME("NewSpaceSim Frame Marker");

            Profiling::Profiler& profiler = Profiling::Profiler::GetInstance();
            profiler.BeginFrame();
            

            m_performanceTimer.update();
            m_elapsedTime = m_performanceTimer.getElapsedTime();
            m_time = m_performanceTimer.getTime();
            m_inputSystem.update(m_elapsedTime, m_time);
            Input input = m_inputSystem.getInput();
            //THis needs to happen in single threaded update
            {
                m_UpdateThread.LockCriticalSection();

                PROFILE_EVENT("SingleThreadedUpdate", Green);
                OPTICK_EVENT("SingleThreadedUpdate");

                //const Camera* cam = m_cameraSystem.getCamera("global");
                //m_view = cam->getCamera();

                m_UpdateThread.setInput(input);

                m_renderSystem.setInput(input);

                m_UpdateThread.SetElapsedTime(m_elapsedTime, m_time);

                m_messageQueues.swapQueues();

                m_UpdateThread.UnblockThread();
                m_UpdateThread.UnLockCriticalSection();
            }
            //Unblock simulation here
            
            //Signal the job system
            m_jobSystem.SignalWorkAvailable();

            m_renderSystem.beginDraw();
            m_renderSystem.update(m_elapsedTime, m_time);
            m_renderSystem.endDraw();

            static size_t updateLoopCount = 0;
            if (updateLoopCount == 2)
            {
                profiler.WriteFile();
            }
            updateLoopCount++;

            InputActions::ActionType inputAction;
            InputSystem::getInputActionFromName("exit_game"_hash, inputAction);
            if (input.getInput(0) && input.getInput(0)->getActionValue(inputAction))
            {
                PostQuitMessage(0);
            }
        }
    }

    m_UpdateThread.stopThread();
}

///-----------------------------------------------------------------------------
///! @brief  Pass win32 messages on to the window
///-----------------------------------------------------------------------------
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
        if (raw != nullptr)
        {
            m_inputDispatch(raw);
        }
    }

    //If the window doesn't exist yet pass on all the messages to the default win32 message handler
    return DefWindowProc (hwnd, message, wParam, lParam);
}

///-----------------------------------------------------------------------------
///! @brief  Cleanup the instance and window
///-----------------------------------------------------------------------------
void Application::cleanup()
{
    m_renderSystem.cleanup();
    m_gameResource->getGameObjectManager().cleanup();
    m_settingsManager.cleanup();

    Profiling::Profiler& profiler = Profiling::Profiler::GetInstance();

    profiler.Cleanup();

    m_physicsManger.Cleanup();

    //Need to add a cleanup call to the cache
    //delete cache;

}

