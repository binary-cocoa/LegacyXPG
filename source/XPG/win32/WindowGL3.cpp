#include "WindowGL3.hpp"
#include <XPG/private/glew.h>
#include <XPG/private/wglew.h>

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
        if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB)
        {
            Window::setupDC();
            return;
        }

        int nPixCount = 0;
        int nPixelFormat  = -1;

        int pixAttribs[] = {
            WGL_SUPPORT_OPENGL_ARB, 1,
            WGL_DRAW_TO_WINDOW_ARB, 1,
            WGL_ACCELERATION_ARB, 1,
            WGL_RED_BITS_ARB, 8,
            WGL_GREEN_BITS_ARB, 8,
            WGL_BLUE_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 16,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            0 };

        wglChoosePixelFormatARB(mHDC, pixAttribs, NULL, 1, &nPixelFormat,
            (UINT*)&nPixCount);

        if (nPixelFormat == -1)
        {
            std::cerr << "wglChoosePixelFormatARB failed\n";
            mHDC = 0;
            return;
        }

        if (mSettings.profile == Context::GL32)
        {
            mSettings.context.vMajor = 3;
            mSettings.context.vMinor = 2;
        }

        GLint attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, mSettings.context.vMajor,
            WGL_CONTEXT_MINOR_VERSION_ARB, mSettings.context.vMinor,
            0 };

        mHGLRC = wglCreateContextAttribsARB(mHDC, 0, attribs);
        if (!mHGLRC)
        {
            std::cerr << "wglCreateContextAttribsARB failed\n";
            return;
        }
    }
}
