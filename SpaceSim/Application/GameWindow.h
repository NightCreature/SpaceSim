#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include "Input/XController.h"
#include "Graphics/Camera.h"

//-----------------------------------------------------------------------------
//! @brief  This class represents the window in which the game is running
//! @remark You need to overload this class and provide an implementation for the update function
//-----------------------------------------------------------------------------
class GameWindow
{
public:
	GameWindow();
    virtual ~GameWindow() 
    {
    }

    bool createWindow( const std::string& applicationName, const std::string windowTitle );
    void showWindow();

    ///Function will be called in between createWindow and showWindow and will setup device and call derived initialise
    void initialise();
    ///Update loop which is called when there are no messages, called from mainGameLoop
    void update(float elapsedTime, double time);

    inline bool hasFocus() const { return m_hasFocus; }
    inline void setWindhowHandle(const HWND& windowHandle) {m_windowHandle = windowHandle;}
    inline const HWND& getWindowHandle() const { return m_windowHandle; }
    inline void setHInstance(const HINSTANCE& hInstance) { m_hInstance = hInstance; }
    inline const HINSTANCE& getHInstance() const {return m_hInstance; }
    inline void setWindowClassExtended(const WNDCLASSEX& windowClassEx) {m_windowClassExtended = windowClassEx;}
    inline const WNDCLASSEX& getWindowClassExtended() const { return m_windowClassExtended; }
    inline bool getDestroyable() const { return m_destroyable; }
    inline void setDestroyable(bool destroyable) { m_destroyable = destroyable; }

    virtual void messageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
    void setFpsInWindowTitle(float elpasedTime);
    unsigned int getFPS() { return m_fps; }
    void calculateFPS(float elapsedTime);

    HINSTANCE m_hInstance;
    HWND m_windowHandle;
    WNDCLASSEX m_windowClassExtended;
    UINT m_message;
    WPARAM m_wParam;
    LPARAM m_lParam;
    static bool m_destroyable;
    std::string m_windowTitle;
    bool m_hasFocus;

    int m_windowWidth;
    int m_windowHeight;

    unsigned int m_fps;
    unsigned int m_framesCounter;
    float        m_timeAmount;
};