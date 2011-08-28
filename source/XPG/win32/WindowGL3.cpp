#include "WindowGL3.hpp"
#include <XPG/private/glew.h>
#include <XPG/private/wglew.h>

#include <fstream>

namespace XPG
{
    WindowGL3::WindowGL3(HINSTANCE inHINSTANCE, Engine::Settings& inSettings,
        Fullscreen::Mode inMode, const char* inTitle) : Window(inHINSTANCE,
        inSettings, inMode, inTitle)
    {
    }

    WindowGL3::~WindowGL3()
    {
    }

    void WindowGL3::setupDC()
    {

    }
}
