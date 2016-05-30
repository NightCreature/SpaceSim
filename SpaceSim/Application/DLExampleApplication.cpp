// DLExampleApplication.cpp : Defines the entry point for the console application.
//


#include "BaseApplication.h"
#include "DeviceManager.h"
#include "SettingsManager.h"
#include "Window.h"
#include "MiscUtil.h"
#include "vld.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    bool useCG = true;
    //Before everything setup the log path
    LogManager* lm = LogManager::getInstance();
    if (lm)
    {
        lm->setLogPath(Util::Path::instance().getLogPath());

        Application& theGame = Application::instance();
        theGame.setupLogChannels();
        theGame.initialise("DLExampleApplication.cfg", false);
        theGame.enumerateInputDevices();
        theGame.mainGameLoop();

        DeviceManager::getInstance().cleanup();
        SettingsManager::getInstance().cleanup();
        theGame.cleanup();

        lm->cleanup();
        delete lm;
    }

	return 0;
}

