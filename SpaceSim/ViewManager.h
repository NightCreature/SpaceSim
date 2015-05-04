#pragma once

#include "vector2.h"

#pragma warning( push )
#pragma warning( disable: 4100 )
#include <EAWebKit/EAWebKit>
#pragma warning( pop )


#include <deque>

class ViewSurface;
class HardwareViewRenderer;

class ViewManager
{
public:
    ViewManager(EA::WebKit::EAWebKitLib* lib, const Vector2& viewSize) : m_eaWebkitLib(lib), m_viewSize(viewSize) {}
    ~ViewManager() {}

    void loadUrl(const std::string& url);

private:
    typedef std::pair<EA::WebKit::View*, ViewSurface*> ViewSurfacePair;
    EA::WebKit::EAWebKitLib* m_eaWebkitLib;
    HardwareViewRenderer* m_hardwareViewRenderer;
    Vector2 m_viewSize;
    std::deque<ViewSurfacePair> m_views;
};

