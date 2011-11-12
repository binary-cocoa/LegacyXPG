#include <XPG/Engine.hpp>
#include <XPG/Platforms.hpp>
#include <XPG/Timer.hpp>

#include "Window.hpp"

#include <cstdlib>
#include <iostream>
using namespace std;

namespace XPG
{
    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

    struct MetaWin32
    {
        Window* window;
        HINSTANCE instance;
    };

    Engine::Engine(const Settings& inSettings) : mMeta(NULL),
        mSettings(inSettings)
    {
        MetaWin32* meta = new MetaWin32;

        meta->instance = GetModuleHandle(NULL);

        WNDCLASS wc;

        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = meta->instance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszMenuName = NULL;
        wc.lpszClassName = ClassName;
        wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);

        ATOM a = RegisterClass(&wc);
        if (!a)
        {
            cerr << "failed to register class\n";
            exit(1);
        }

        if (mSettings.profile != Context::Legacy)
        {
            Window w(meta->instance); // temporary useless window
        }

        meta->window = new Window(meta->instance);

        const GLubyte* s = glGetString(GL_VERSION);
        //cout << "GL version: " << s << endl;
        mSettings.context.vMajor = s[0] - '0';
        mSettings.context.vMinor = s[2] - '0';

        if (mSettings.context.vMajor >= 2)
        {
            s = glGetString(GL_SHADING_LANGUAGE_VERSION);
            mSettings.shader.vMajor = s[0] - '0';
            mSettings.shader.vMinor = (s[2] - '0') * 10 + (s[3] - '0');
        }
        else
        {
            mSettings.shader.vMajor = 0;
            mSettings.shader.vMinor = 0;
        }

        mMeta = meta;
    }

    Engine::~Engine()
    {
        MetaWin32* meta = reinterpret_cast<MetaWin32*>(mMeta);
        delete meta;
    }

    void Engine::swapBuffers()
    {
        MetaWin32* meta = reinterpret_cast<MetaWin32*>(mMeta);
        meta->window->swapBuffers();
    }

    bool Engine::getEvent(Event& inEvent)
    {
        MetaWin32* meta = reinterpret_cast<MetaWin32*>(mMeta);
        return meta->window->getEvent(inEvent);
    }

    void Engine::runModule(Module& inModule)
    {
        if (!mSettings.width) return;

        //meta->module = inModule;

        Event event;
        event.type = Event::Window;
        event.window.event = WindowEvent::Resize;
        event.window.resize = WindowEvent::Regular;
        event.window.width = mSettings.width;
        event.window.height = mSettings.height;
        inModule.handleEvent(event);
        inModule.startRunning();

        while (inModule.isRunning())
        {
            while (getEvent(event)) inModule.handleEvent(event);
            inModule.onDisplay();
            swapBuffers();
            Idle(1);
        }
    }

    void Engine::setWindowTitle(const char* inTitle)
    {
    }

    void Engine::setIconTitle(const char* inTitle)
    {
        /// unavailable in Win32 (?)
    }

    void Engine::setFullscreen(Fullscreen::Mode inMode)
    {
    }
}
