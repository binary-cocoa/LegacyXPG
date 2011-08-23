#include <XPG/Engine.hpp>
#include <XPG/Timer.hpp>

#include <XPG/private/glew.h>
#include <XPG/private/glxew.h>
#include <X11/Xatom.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;

namespace XPG
{
    Key::Code getKeyCode(int inIndex);

    void setGLXFunctionPointers()
    {
        glGenVertexArraysAPPLE = (void(*)(GLsizei, const GLuint*))
            glXGetProcAddressARB((GLubyte*)"glGenVertexArrays");
        glBindVertexArrayAPPLE = (void(*)(const GLuint))
            glXGetProcAddressARB((GLubyte*)"glBindVertexArray");
        glDeleteVertexArraysAPPLE = (void(*)(GLsizei, const GLuint*))
            glXGetProcAddressARB((GLubyte*)"glGenVertexArrays");

        glXCreateContextAttribsARB = (GLXContext(*)(Display*, GLXFBConfig,
            GLXContext, Bool, const int*))glXGetProcAddressARB
            ((GLubyte*)"glXCreateContextAttribsARB");
        glXChooseFBConfig = (GLXFBConfig*(*)(Display*, int, const int*, int*))
            glXGetProcAddressARB((GLubyte*)"glXChooseFBConfig");
        glXGetVisualFromFBConfig = (XVisualInfo*(*)(Display*, GLXFBConfig))
            glXGetProcAddressARB((GLubyte*)"glXGetVisualFromFBConfig");
    }

    struct Engine::PrivateData
    {
        GLXContext context;
        Display* display;
        Window window;
        long eventMask;
        Atom wmDeleteMessage;
    };

    void Engine::begin()
    {
        mData = new PrivateData;
        mActive = true;

        XSetWindowAttributes winAttribs;
        GLint winmask;
        GLint MajorGLX = 0;
        GLint MinorGLX = 0;
        XVisualInfo* visualInfo;
        GLXFBConfig* fbConfigs;
        int numConfigs = 0;
        static int fbAttribs[] =
            {
                GLX_RENDER_TYPE,   GLX_RGBA_BIT,
                GLX_X_RENDERABLE,  True,
                GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                GLX_DOUBLEBUFFER,  True,
                GLX_RED_SIZE,      8,
                GLX_BLUE_SIZE,     8,
                GLX_GREEN_SIZE,    8,
                0
            };

        setGLXFunctionPointers();

        mData->display = XOpenDisplay(NULL);

        glXQueryVersion(mData->display, &MajorGLX, &MinorGLX);
        cout << "supported GLX version: " << MajorGLX << '.' << MinorGLX
            << endl;

        if(MajorGLX == 1 && MinorGLX < 2)
        {
            cerr << "ERROR -- GLX 1.2 or greater is required\n";
            XCloseDisplay(mData->display);

            mActive = false;
            return;
        }

        fbConfigs = glXChooseFBConfig(mData->display,
            DefaultScreen(mData->display), fbAttribs, &numConfigs);
        visualInfo = glXGetVisualFromFBConfig(mData->display, fbConfigs[0]);

        /// X window creation
        mData->eventMask = ExposureMask | VisibilityChangeMask |
            KeyPressMask | PointerMotionMask | StructureNotifyMask
            | ButtonPressMask | ButtonReleaseMask | FocusChangeMask
            | EnterWindowMask | LeaveWindowMask | KeyReleaseMask;
        winAttribs.event_mask = mData->eventMask;

        winAttribs.border_pixel = 0;
        winAttribs.bit_gravity = StaticGravity;
        winAttribs.colormap = XCreateColormap(mData->display,
            RootWindow(mData->display, visualInfo->screen), visualInfo->visual,
            AllocNone);
        winmask = CWBorderPixel | CWBitGravity | CWEventMask| CWColormap;

        int d = mSettings.depth ? mSettings.depth : visualInfo->depth;
        mData->window = XCreateWindow(mData->display,
            DefaultRootWindow(mData->display), 20, 20, mSettings.width,
            mSettings.height, 0, d, InputOutput, visualInfo->visual, winmask,
            &winAttribs);

        // Allow XPG to handle the closing of the window.
        mData->wmDeleteMessage = XInternAtom(mData->display, "WM_DELETE_WINDOW",
            False);
        XSetWMProtocols(mData->display, mData->window,
            &mData->wmDeleteMessage, 1);

        XMapWindow(mData->display, mData->window);

        if (!mSettings.context.vMajor)
        {
            mSettings.context.vMajor = 3;
            mSettings.context.vMinor = 3;
        }

        /// GL context creation
        GLint attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, mSettings.context.vMajor,
            GLX_CONTEXT_MINOR_VERSION_ARB, mSettings.context.vMinor,
            GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            0
        };

        // Must first check to see if the extension to create an OpenGL 3.x
        // context is even available
        if (glXCreateContextAttribsARB && !mSettings.legacyContext)
        {
            // Good to go. Create the 3.x context.
            mData->context = glXCreateContextAttribsARB(mData->display,
                fbConfigs[0], 0, True, attribs);
        }
        else
        {
            // No good. Create a legacy context.
            mData->context = glXCreateContext(mData->display, visualInfo, NULL,
                True);
            mSettings.legacyContext = true;
        }

        glXMakeCurrent(mData->display, mData->window, mData->context);

        GLenum e = glewInit();
        if (e != GLEW_OK)
            cerr << "ERROR (GLEW) -- " << glewGetErrorString(e);

        glGetError(); // for some reason, a GL_INVALID_ENUM strikes here

        // Somehow, functions get lost in the OpenGL 3.x setup. This
        // updates the pointers to the right functions.
        if (!glGenerateMipmap)
            glGenerateMipmap = (void(*)(GLenum))
                glXGetProcAddressARB((GLubyte*)"glGenerateMipmap");

        if (!glGenVertexArrays)
            glGenVertexArrays = (void(*)(GLsizei, GLuint*))
                glGenVertexArraysAPPLE;

        if (!glDeleteVertexArrays)
            glDeleteVertexArrays = glDeleteVertexArraysAPPLE;

        if (!glBindVertexArray)
            glBindVertexArray = glBindVertexArrayAPPLE;

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

        glViewport(0, 0, mSettings.width, mSettings.height);
    }

    void Engine::end()
    {
        if (mActive)
        {
            glXMakeCurrent(mData->display, None, NULL);
            glXDestroyContext(mData->display, mData->context);
            XDestroyWindow(mData->display, mData->window);
            XCloseDisplay(mData->display);
            mSettings.width = 0;
            mSettings.height = 0;
            mSettings.depth = 0;
            mActive = false;
        }
    }

    void Engine::swapBuffers()
    {
        glXSwapBuffers(mData->display, mData->window);
    }

    bool Engine::getEvent(Event& inEvent)
    {
        if (!XEventsQueued(mData->display, QueuedAfterReading))
            return false;

        XEvent event;
        XNextEvent(mData->display, &event);

        switch(event.type)
        {
            case ButtonPress:
            {
                inEvent.type = Event::Mouse;
                switch (event.xbutton.button)
                {
                    case Button1:
                        inEvent.mouse.event = MouseEvent::ButtonDown;
                        inEvent.mouse.button = MouseEvent::LeftButton;
                        break;

                    case Button2:
                        inEvent.mouse.event = MouseEvent::ButtonDown;
                        inEvent.mouse.button = MouseEvent::MiddleButton;
                        break;

                    case Button3:
                        inEvent.mouse.event = MouseEvent::ButtonDown;
                        inEvent.mouse.button = MouseEvent::RightButton;
                        break;

                    case Button4:
                        inEvent.mouse.event = MouseEvent::WheelUp;
                        break;

                    case Button5:
                        inEvent.mouse.event = MouseEvent::WheelDown;
                        break;

                    default:
                        inEvent.mouse.event = MouseEvent::ButtonDown;
                        inEvent.mouse.button = event.xbutton.button;
                }

                break;
            }

            case ButtonRelease:
            {
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::ButtonUp;

                switch (event.xbutton.button)
                {
                    case Button1:
                        inEvent.mouse.button = MouseEvent::LeftButton;
                        break;

                    case Button2:
                        inEvent.mouse.button = MouseEvent::MiddleButton;
                        break;

                    case Button3:
                        inEvent.mouse.button = MouseEvent::RightButton;
                        break;

                    case Button4:
                    case Button5:
                        inEvent.type = Event::NoEvent;
                        break;

                    default:
                        inEvent.mouse.event = MouseEvent::ButtonDown;
                        inEvent.mouse.button = event.xbutton.button;
                }

                break;
            }

            case FocusIn:
            {
                inEvent.type = Event::Window;
                inEvent.window.event = WindowEvent::Focus;
                break;
            }

            case FocusOut:
            {
                inEvent.type = Event::Window;
                inEvent.window.event = WindowEvent::Blur;
                break;
            }

            case EnterNotify:
            {
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::EnterWindow;
                break;
            }

            case LeaveNotify:
            {
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::LeaveWindow;
                break;
            }

            case UnmapNotify:
            {
                break;
            }

            case MapNotify:
            case ConfigureNotify:
            {
                XWindowAttributes winData;
                XGetWindowAttributes(mData->display, mData->window,
                    &winData);
                mSettings.height = winData.height;
                mSettings.width = winData.width;
                glViewport(0, 0, mSettings.width, mSettings.height);
                inEvent.type = Event::Window;
                inEvent.window.event = WindowEvent::Resize;
                inEvent.window.width = mSettings.width;
                inEvent.window.height = mSettings.height;
                break;
            }

            case ClientMessage:
            {
                if (event.xclient.format == 32 &&
                    static_cast<Atom>(event.xclient.data.l[0])
                        == mData->wmDeleteMessage)
                {
                    inEvent.type = Event::Window;
                    inEvent.window.event = WindowEvent::Exit;
                }
                break;
            }

            case MotionNotify:
            {
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::Motion;
                inEvent.mouse.x = event.xmotion.x;
                inEvent.mouse.y = event.xmotion.y;
                break;
            }

            case KeyPress:
            {
                inEvent.type = Event::Keyboard;
                inEvent.keyboard.event = KeyboardEvent::Press;
                inEvent.keyboard.key = getKeyCode(event.xkey.keycode);
                break;
            }

            case KeyRelease:
            {
                bool isRepeat = false;
                inEvent.type = Event::Keyboard;
                inEvent.keyboard.key = getKeyCode(event.xkey.keycode);

                // X11 sends both a KeyPress and a KeyRelease when repeating
                // a key. The code below distinguishes between a true key
                // release and a key repeat. If it is a key repeat, it
                // properly disposes of the subsequent KeyPress message.
                if (XEventsQueued(mData->display, QueuedAfterReading))
                {
                    XEvent e;
                    XPeekEvent(mData->display, &e);
                    if (e.type == KeyPress
                        && e.xkey.time == event.xkey.time
                        && e.xkey.keycode == event.xkey.keycode)
                    {
                        inEvent.keyboard.event = KeyboardEvent::Repeat;

                        // dispose of the next event
                        XNextEvent(mData->display, &e);

                        isRepeat = true;
                    }
                }

                if (!isRepeat)
                {
                    inEvent.keyboard.event = KeyboardEvent::Release;
                }

                break;
            }

            case DestroyNotify:
            {
                cout << "destroy" << endl;
                break;
            }

            default:
            {
                //cout << "no event?" << endl;
            }
        }

        return true;
    }

    void Engine::runModule(Module& inModule)
    {
        if (!mActive) return;

        Event event;
        event.type = Event::Window;
        event.window.event = WindowEvent::Resize;
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
        if (!mActive || !inTitle) return;

        XTextProperty titleProperty;
        Status status;

        size_t length = strlen(inTitle);
        char* t = new char[length + 1];
        strcpy(t, inTitle);

        status = XStringListToTextProperty(&t, 1, &titleProperty);
        if (status)
        {
            XSetTextProperty(mData->display, mData->window, &titleProperty,
                XA_WM_NAME);
            XFree(titleProperty.value);
        }

        delete [] t;
    }

    void Engine::setIconTitle(const char* inTitle)
    {
        if (!mActive || !inTitle) return;

        XTextProperty titleProperty;
        Status status;

        size_t length = strlen(inTitle);
        char* t = new char[length + 1];
        strcpy(t, inTitle);

        status = XStringListToTextProperty(&t, 1, &titleProperty);
        if (status)
        {
            XSetTextProperty(mData->display, mData->window, &titleProperty,
                XA_WM_ICON_NAME);
            XFree(titleProperty.value);
        }

        delete [] t;
    }

    Key::Code getKeyCode(int inIndex)
    {
        switch (inIndex)
        {
            case 38: return Key::LetterA;
            case 56: return Key::LetterB;
            case 54: return Key::LetterC;
            case 40: return Key::LetterD;
            case 26: return Key::LetterE;
            case 41: return Key::LetterF;
            case 42: return Key::LetterG;
            case 43: return Key::LetterH;
            case 31: return Key::LetterI;
            case 44: return Key::LetterJ;
            case 45: return Key::LetterK;
            case 46: return Key::LetterL;
            case 58: return Key::LetterM;
            case 57: return Key::LetterN;
            case 32: return Key::LetterO;
            case 33: return Key::LetterP;
            case 24: return Key::LetterQ;
            case 27: return Key::LetterR;
            case 39: return Key::LetterS;
            case 28: return Key::LetterT;
            case 30: return Key::LetterU;
            case 55: return Key::LetterV;
            case 25: return Key::LetterW;
            case 53: return Key::LetterX;
            case 29: return Key::LetterY;
            case 52: return Key::LetterZ;

            case 19: return Key::TopRow0;
            case 10: return Key::TopRow1;
            case 11: return Key::TopRow2;
            case 12: return Key::TopRow3;
            case 13: return Key::TopRow4;
            case 14: return Key::TopRow5;
            case 15: return Key::TopRow6;
            case 16: return Key::TopRow7;
            case 17: return Key::TopRow8;
            case 18: return Key::TopRow9;

            case 67: return Key::F1;
            case 68: return Key::F2;
            case 69: return Key::F3;
            case 70: return Key::F4;
            case 71: return Key::F5;
            case 72: return Key::F6;
            case 73: return Key::F7;
            case 74: return Key::F8;
            case 75: return Key::F9;
            case 76: return Key::F10;
            case 95: return Key::F11;
            case 96: return Key::F12;

            case 9: return Key::Escape;
            case 49: return Key::BackQuote;
            case 20: return Key::Minus;
            case 21: return Key::Equals;
            case 22: return Key::Backspace;
            case 23: return Key::Tab;
            case 34: return Key::LeftBracket;
            case 35: return Key::RightBracket;
            case 51: return Key::Backslash;
            case 66: return Key::CapsLock;
            case 47: return Key::Semicolon;
            case 48: return Key::Quote;
            case 36: return Key::Enter;
            case 59: return Key::Comma;
            case 60: return Key::Period;
            case 61: return Key::Slash;
            case 65: return Key::Space;

            case 50: return Key::LeftShift;
            case 62: return Key::RightShift;
            case 37: return Key::LeftControl;
            case 105: return Key::RightControl;
            case 64: return Key::LeftAlt;
            case 108: return Key::RightAlt;
            case 133: return Key::LeftSuper;
            case 134: return Key::RightSuper;

            case 78: return Key::ScrollLock;
            case 127: return Key::Pause;
            // need key code for Print Screen, if possible

            case 118: return Key::Insert;
            case 110: return Key::Home;
            case 112: return Key::PageUp;
            case 117: return Key::PageDown;
            case 115: return Key::End;
            case 119: return Key::Delete;

            case 111: return Key::Up;
            case 116: return Key::Down;
            case 113: return Key::Left;
            case 114: return Key::Right;

            case 90: return Key::Keypad0;
            case 87: return Key::Keypad1;
            case 88: return Key::Keypad2;
            case 89: return Key::Keypad3;
            case 83: return Key::Keypad4;
            case 84: return Key::Keypad5;
            case 85: return Key::Keypad6;
            case 79: return Key::Keypad7;
            case 80: return Key::Keypad8;
            case 81: return Key::Keypad9;

            case 77: return Key::NumLock;
            case 106: return Key::KeypadSlash;
            case 63: return Key::KeypadAsterisk;
            case 82: return Key::KeypadMinus;
            case 86: return Key::KeypadPlus;
            case 104: return Key::KeypadEnter;
            case 91: return Key::KeypadPeriod;

            default: return Key::Unknown;
        }
    }
}
