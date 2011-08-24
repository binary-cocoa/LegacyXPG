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
    static const size_t NumMappings = 135;
    static Key::Code KeyTable[NumMappings] = {
        Key::Unknown, // 0
        Key::Unknown, // 1
        Key::Unknown, // 2
        Key::Unknown, // 3
        Key::Unknown, // 4
        Key::Unknown, // 5
        Key::Unknown, // 6
        Key::Unknown, // 7
        Key::Unknown, // 8
        Key::Escape, // 9
        Key::TopRow1, // 10
        Key::TopRow2, // 11
        Key::TopRow3, // 12
        Key::TopRow4, // 13
        Key::TopRow5, // 14
        Key::TopRow6, // 15
        Key::TopRow7, // 16
        Key::TopRow8, // 17
        Key::TopRow9, // 18
        Key::TopRow0, // 19
        Key::Minus, // 20
        Key::Equals, // 21
        Key::Backspace, // 22
        Key::Tab, // 23
        Key::LetterQ, // 24
        Key::LetterW, // 25
        Key::LetterE, // 26
        Key::LetterR, // 27
        Key::LetterT, // 28
        Key::LetterY, // 29
        Key::LetterU, // 30
        Key::LetterI, // 31
        Key::LetterO, // 32
        Key::LetterP, // 33
        Key::LeftBracket, // 34
        Key::RightBracket, // 35
        Key::Enter, // 36
        Key::LeftControl, // 37
        Key::LetterA, // 38
        Key::LetterS, // 39
        Key::LetterD, // 40
        Key::LetterF, // 41
        Key::LetterG, // 42
        Key::LetterH, // 43
        Key::LetterJ, // 44
        Key::LetterK, // 45
        Key::LetterL, // 46
        Key::Semicolon, // 47
        Key::Quote, // 48
        Key::BackQuote, // 49
        Key::LeftShift, // 50
        Key::Backslash, // 51
        Key::LetterZ, // 52
        Key::LetterX, // 53
        Key::LetterC, // 54
        Key::LetterV, // 55
        Key::LetterB, // 56
        Key::LetterN, // 57
        Key::LetterM, // 58
        Key::Comma, // 59
        Key::Period, // 60
        Key::Slash, // 61
        Key::RightShift, // 62
        Key::KeypadAsterisk, // 63
        Key::LeftAlt, // 64
        Key::Space, // 65
        Key::CapsLock, // 66
        Key::F1, // 67
        Key::F2, // 68
        Key::F3, // 69
        Key::F4, // 70
        Key::F5, // 71
        Key::F6, // 72
        Key::F7, // 73
        Key::F8, // 74
        Key::F9, // 75
        Key::F10, // 76
        Key::NumLock, // 77
        Key::ScrollLock, // 78
        Key::Keypad7, // 79
        Key::Keypad8, // 80
        Key::Keypad9, // 81
        Key::KeypadMinus, // 82
        Key::Keypad4, // 83
        Key::Keypad5, // 84
        Key::Keypad6, // 85
        Key::KeypadPlus, // 86
        Key::Keypad1, // 87
        Key::Keypad2, // 88
        Key::Keypad3, // 89
        Key::Keypad0, // 90
        Key::KeypadPeriod, // 91
        Key::Unknown, // 92
        Key::Unknown, // 93
        Key::Unknown, // 94
        Key::F11, // 95
        Key::F12, // 96
        Key::Unknown, // 97
        Key::Unknown, // 98
        Key::Unknown, // 99
        Key::Unknown, // 100
        Key::Unknown, // 101
        Key::Unknown, // 102
        Key::Unknown, // 103
        Key::KeypadEnter, // 104
        Key::RightControl, // 105
        Key::KeypadSlash, // 106
        Key::Unknown, // 107
        Key::RightAlt, // 108
        Key::Unknown, // 109
        Key::Home, // 110
        Key::Up, // 111
        Key::PageUp, // 112
        Key::Left, // 113
        Key::Right, // 114
        Key::End, // 115
        Key::Down, // 116
        Key::PageDown, // 117
        Key::Insert, // 118
        Key::Delete, // 119
        Key::Unknown, // 120
        Key::Unknown, // 121
        Key::Unknown, // 122
        Key::Unknown, // 123
        Key::Unknown, // 124
        Key::Unknown, // 125
        Key::Unknown, // 126
        Key::Pause, // 127
        Key::Unknown, // 128
        Key::Unknown, // 129
        Key::Unknown, // 130
        Key::Unknown, // 131
        Key::Unknown, // 132
        Key::LeftSuper, // 133
        Key::RightSuper, // 134
        };

    static inline Key::Code lookupKey(int inCode)
    {
        return inCode >= 0 && inCode < NumMappings ? KeyTable[inCode]
            : Key::Unknown;
    }

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
                inEvent.keyboard.key = lookupKey(event.xkey.keycode);
                break;
            }

            case KeyRelease:
            {
                bool isRepeat = false;
                inEvent.type = Event::Keyboard;
                inEvent.keyboard.key = lookupKey(event.xkey.keycode);

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
}
