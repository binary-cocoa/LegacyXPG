#include <XPG/Engine.hpp>
#include <XPG/Platforms.hpp>
#include <XPG/Timer.hpp>

#include <XPG/private/windows.hpp>
#include <XPG/private/glew.h>
#include <XPG/private/wglew.h>

#include "WindowGL3.hpp"

#include <iostream>
using namespace std;

namespace XPG
{
    struct MetaWin32
    {
        bool active;
        bool dispatchResize;
        int formerWidth;
        int formerHeight;
        Window* window;
        char title[255];
        Module* module;
    };

    Engine::Engine(const Settings& inSettings) : mSettings(inSettings)
    {
        MetaWin32* meta = new MetaWin32;
        mMeta = meta;

        meta->active = false;
        meta->dispatchResize = false;
        meta->formerWidth = 640;
        meta->formerHeight = 360;

        strcpy(meta->title, "OpenGL 3");

        if (meta->active) return;

        meta->active = true;

        HINSTANCE hi = GetModuleHandle(NULL);

        Window* w = new Window(hi, mSettings,
            mSettings.fullscreen, meta->title);

        GLenum e = glewInit();
        if (e != GLEW_OK)
        {
            cerr << "failed to init GLEW" << endl;
            return;
        }

        if (mSettings.profile != Context::Legacy)
        {
            delete w;
            w = new WindowGL3(hi, mSettings, mSettings.fullscreen, meta->title);
        }

        meta->window = w;

        meta->window->show(SW_SHOW);
        meta->window->update();
    }

    Engine::~Engine()
    {
        MetaWin32* meta = reinterpret_cast<MetaWin32*>(mMeta);
        if (meta->active)
        {
            delete meta->window;
            mSettings.width = 0;
            mSettings.height = 0;
            mSettings.depth = 0;
            meta->active = false;
            //PostQuitMessage(0);
        }

        delete meta;
        mMeta = NULL;
    }

    void Engine::swapBuffers()
    {
        MetaWin32* meta = reinterpret_cast<MetaWin32*>(mMeta);
        meta->window->swapBuffers();
    }

    bool Engine::getEvent(Event& inEvent)
    {
        MetaWin32* meta = reinterpret_cast<MetaWin32*>(mMeta);
        inEvent.type = Event::NoEvent;

        if (meta->dispatchResize)
        {
            meta->dispatchResize = false;
            inEvent.type = Event::Window;
            inEvent.window.event = WindowEvent::Resize;
            inEvent.window.resize = WindowEvent::Regular;

            RECT rc;
            meta->window->querySize(rc);
            mSettings.width = rc.right - rc.left;
            mSettings.height = rc.bottom - rc.top;

            inEvent.window.width = mSettings.width;
            inEvent.window.height = mSettings.height;
            return true;
        }

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
        MetaWin32* meta = reinterpret_cast<MetaWin32*>(mMeta);
        meta->window->setTitle(inTitle);
    }

    void Engine::setIconTitle(const char* inTitle)
    {
        /// unavailable in Win32 (?)
    }

    void Engine::setFullscreen(Fullscreen::Mode inMode)
    {
        MetaWin32* meta = reinterpret_cast<MetaWin32*>(mMeta);
        switch (inMode)
        {
            case Fullscreen::Hard:
            {
                return; // not yet supported
                break;
            }

            case Fullscreen::Soft:
            {
                if (mSettings.fullscreen == Fullscreen::Off)
                    meta->window->softFullscreen(true);
                break;
            }

            case Fullscreen::Off:
            {
                if (mSettings.fullscreen == Fullscreen::Soft)
                    meta->window->softFullscreen(false);
                break;
            }

            default: {} // lolwut
        }

        meta->dispatchResize = true;
        mSettings.fullscreen = inMode;
    }

    /// /// ///


}
