#pragma once

#include "ICommand.h"
#include "ViewManager.h"

#pragma warning( push )
#pragma warning( disable: 4100 )
#include <EAWebKit/EAWebKit>
#pragma warning( pop )

//Place these two in their own files
class ViewSurface : public EA::WebKit::ISurface
{};

class HardwareViewRenderer : public EA::WebKit::IHardwareRenderer
{};

class WebkitCommand : public ICommand
{
public:
    WebkitCommand(ICommandData* commandData) : ICommand(commandData) {}
    void setWebkitLibPtr(EA::WebKit::EAWebKitLib* lib) { m_webkitLib = lib; }
protected:
    EA::WebKit::EAWebKitLib* m_webkitLib; //Needed for things in webkit
};

class CreateViewCommand : public WebkitCommand
{
public:
    CreateViewCommand(ICommandData* commandData) : WebkitCommand(commandData) {}

    struct CreateViewCommandData : public ICommandData
    {
        EA::WebKit::View* m_view;//output pointer for new view
        Vector2 m_viewSize;
        ViewSurface* m_surface;
        HardwareViewRenderer* m_hardwareViewRenderer;
        ViewManager* m_viewManager;
    };

    virtual bool RunCommand() override
    {
        CreateViewCommandData* commandData = static_cast<CreateViewCommandData*>(m_commandData);
        commandData->m_view = m_webkitLib->CreateView();
        EA::WebKit::ViewParameters parameters;
        parameters.mWidth = static_cast<int>(commandData->m_viewSize.x());
        parameters.mHeight = static_cast<int>(commandData->m_viewSize.y());
        parameters.mDisplaySurface = static_cast<EA::WebKit::ISurface*>(commandData->m_surface);
        parameters.mHardwareRenderer = static_cast<EA::WebKit::IHardwareRenderer*>(commandData->m_hardwareViewRenderer);
        commandData->m_view->InitView(parameters);
        ViewManager::ViewSurfacePair pair;
        pair.first = commandData->m_view;
        pair.second = commandData->m_surface;
        commandData->m_viewManager->AddView(pair);
    }

};

class LoadUrlCommand : public WebkitCommand
{
public:
    LoadUrlCommand(ICommandData* commandData) : WebkitCommand(commandData) {}

    struct LoadUrlCommandData : public ICommandData
    {
        EA::WebKit::View* m_view;
        std::string m_urlToLoad;
    };

    virtual bool RunCommand() override
    {
        LoadUrlCommandData* commandData = static_cast<LoadUrlCommandData*>(m_commandData);
        commandData->m_view->SetURI(commandData->m_urlToLoad.c_str());
    }

};