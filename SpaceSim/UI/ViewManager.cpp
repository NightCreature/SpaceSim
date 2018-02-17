//#include "ViewManager.h"
//
//#include "StringHelperFunctions.h"
//
/////-----------------------------------------------------------------------------
/////! @brief   TODO enter a description
/////! @remark
/////-----------------------------------------------------------------------------
//void ViewManager::loadUrl(const std::string& url)
//{
//    EA::WebKit::View* view = m_eaWebkitLib->CreateView();
//
//    EA::WebKit::ViewParameters params;
//    params.mBackgroundColor = 0xFFFFFFFF; //Solid White background
//    params.mWidth = static_cast<int>(m_viewSize.x());
//    params.mHeight = static_cast<int>(m_viewSize.y());
//    //params.mDisplaySurface = new ViewSurface(m_viewSize);
//    //params.mHardwareRenderer = m_hardwareViewRenderer;
//    if (!view->InitView(params))
//    {
//        MSG_TRACE_CHANNEL("ViewManager", "Failed to initalise the view");
//        return;
//    }
//
//    view->SetURI(url.c_str());
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   TODO enter a description
/////! @remark
/////-----------------------------------------------------------------------------
//void ViewManager::AddView(ViewSurfacePair& viewPair)
//{
//    m_views.push_back(viewPair);
//}
