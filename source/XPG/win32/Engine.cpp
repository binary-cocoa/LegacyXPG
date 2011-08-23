#include <XPG/Engine.hpp>
#include <XPG/Platforms.hpp>
#include <XPG/Timer.hpp>

#include <XPG/private/windows.hpp>
#include <XPG/private/glew.h>
#include <XPG/private/wglew.h>

#include <iostream>
using namespace std;

namespace XPG
{
    struct KeyMapping
    {
        Key::Code normal;
        Key::Code extended;
    };

    static const size_t NumMappings = 93;
    static const KeyMapping KeyTable[NumMappings] = {
        { Key::Unknown, Key::Unknown },                 // 0
        { Key::Escape, Key::Escape },                   // 1
        { Key::TopRow1, Key::TopRow1 },                 // 2
        { Key::TopRow2, Key::TopRow2 },                 // 3
        { Key::TopRow3, Key::TopRow3 },                 // 4
        { Key::TopRow4, Key::TopRow4 },                 // 5
        { Key::TopRow5, Key::TopRow5 },                 // 6
        { Key::TopRow6, Key::TopRow6 },                 // 7
        { Key::TopRow7, Key::TopRow7 },                 // 8
        { Key::TopRow8, Key::TopRow8 },                 // 9
        { Key::TopRow9, Key::TopRow9 },                 // 10
        { Key::TopRow0, Key::TopRow0 },                 // 11
        { Key::Minus, Key::Minus },                     // 12
        { Key::Equals, Key::Equals },                   // 13
        { Key::Backspace, Key::Backspace },             // 14
        { Key::Tab, Key::Tab },                         // 15
        { Key::LetterQ, Key::LetterQ },                 // 16
        { Key::LetterW, Key::LetterW },                 // 17
        { Key::LetterE, Key::LetterE },                 // 18
        { Key::LetterR, Key::LetterR },                 // 19
        { Key::LetterT, Key::LetterT },                 // 20
        { Key::LetterY, Key::LetterY },                 // 21
        { Key::LetterU, Key::LetterU },                 // 22
        { Key::LetterI, Key::LetterI },                 // 23
        { Key::LetterO, Key::LetterO },                 // 24
        { Key::LetterP, Key::LetterP },                 // 25
        { Key::LeftBracket, Key::LeftBracket },         // 26
        { Key::RightBracket, Key::RightBracket },       // 27
        { Key::Enter, Key::KeypadEnter },               // 28
        { Key::LeftControl, Key::RightControl },        // 29
        { Key::LetterA, Key::LetterA },                 // 30
        { Key::LetterS, Key::LetterS },                 // 31
        { Key::LetterD, Key::LetterD },                 // 32
        { Key::LetterF, Key::LetterF },                 // 33
        { Key::LetterG, Key::LetterG },                 // 34
        { Key::LetterH, Key::LetterH },                 // 35
        { Key::LetterJ, Key::LetterJ },                 // 36
        { Key::LetterK, Key::LetterK },                 // 37
        { Key::LetterL, Key::LetterL },                 // 38
        { Key::Semicolon, Key::Semicolon },             // 39
        { Key::Quote, Key::Quote },                     // 40
        { Key::BackQuote, Key::BackQuote },             // 41
        { Key::LeftShift, Key::LeftShift },             // 42
        { Key::Backslash, Key::Backslash },             // 43
        { Key::LetterZ, Key::LetterZ },                 // 44
        { Key::LetterX, Key::LetterX },                 // 45
        { Key::LetterC, Key::LetterC },                 // 46
        { Key::LetterV, Key::LetterV },                 // 47
        { Key::LetterB, Key::LetterB },                 // 48
        { Key::LetterN, Key::LetterN },                 // 49
        { Key::LetterM, Key::LetterM },                 // 50
        { Key::Comma, Key::Comma },                     // 51
        { Key::Period, Key::Period },                   // 52
        { Key::Slash, Key::KeypadSlash },               // 53
        { Key::RightShift, Key::RightShift },           // 54
        { Key::KeypadAsterisk, Key::KeypadAsterisk },   // 55
        { Key::LeftAlt, Key::RightAlt },                // 56
        { Key::Space, Key::Space },                     // 57
        { Key::CapsLock, Key::CapsLock },               // 58
        { Key::F1, Key::F1 },                           // 59
        { Key::F2, Key::F2 },                           // 60
        { Key::F3, Key::F3 },                           // 61
        { Key::F4, Key::F4 },                           // 62
        { Key::F5, Key::F5 },                           // 63
        { Key::F6, Key::F6 },                           // 64
        { Key::F7, Key::F7 },                           // 65
        { Key::F8, Key::F8 },                           // 66
        { Key::F9, Key::F9 },                           // 67
        { Key::F10, Key::F10 },                         // 68
        { Key::Pause, Key::NumLock },                   // 69
        { Key::ScrollLock, Key::ScrollLock },           // 70
        { Key::Keypad7, Key::Home },                    // 71
        { Key::Keypad8, Key::Up },                      // 72
        { Key::Keypad9, Key::PageUp },                  // 73
        { Key::KeypadSlash, Key::KeypadSlash },         // 74
        { Key::Keypad4, Key::Left },                    // 75
        { Key::Keypad5, Key::Keypad5 },                 // 76
        { Key::Keypad6, Key::Right },                   // 77
        { Key::KeypadPlus, Key::KeypadPlus },           // 78
        { Key::Keypad1, Key::End },                     // 79
        { Key::Keypad2, Key::Down },                    // 80
        { Key::Keypad3, Key::PageDown },                // 81
        { Key::Keypad0, Key::Insert },                  // 82
        { Key::KeypadPeriod, Key::Delete },             // 83
        { Key::Unknown, Key::Unknown },                 // 84
        { Key::Unknown, Key::Unknown },                 // 85
        { Key::Unknown, Key::Unknown },                 // 86
        { Key::F11, Key::F11 },                         // 87
        { Key::F12, Key::F12 },                         // 88
        { Key::Unknown, Key::Unknown },                 // 89
        { Key::Unknown, Key::Unknown },                 // 90
        { Key::LeftSuper, Key::LeftSuper },             // 91
        { Key::RightSuper, Key::RightSuper },           // 92
        };

    static inline Key::Code lookupKey(unsigned int inCode, bool inExtended)
    {
        return inCode < NumMappings ?
            (inExtended ? KeyTable[inCode].extended : KeyTable[inCode].normal)
            : Key::Unknown;
    }

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

    /// evil globals (working on a way to avoid needing these)
    static uint16* activeWidth;
    static uint16* activeHeight;
    static Event* activeEvent = NULL;

    struct Engine::PrivateData
    {
        bool active;
        HWND hWnd;
        HGLRC hrc;
        HDC hdc;
        HINSTANCE hInstance;
        char title[255];
        Module* module;
    };

    void Engine::begin()
    {
        mData = new PrivateData;
        mData->active = false;
        strcpy(mData->title, "OpenGL 3");

        if (mData->active) return;

        mData->active = true;

        mData->hInstance = GetModuleHandle(NULL);
        WNDCLASS windowClass;
        DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

        windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        windowClass.lpfnWndProc = (WNDPROC) WndProc;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = 0;
        windowClass.hInstance = mData->hInstance;
        windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.hbrBackground = NULL;
        windowClass.lpszMenuName = NULL;
        windowClass.lpszClassName = mData->title;

        if (!RegisterClass(&windowClass))
        {
            cerr << "fail on RegisterClass\n";
            return;
        }

        mData->hWnd = CreateWindowEx(dwExStyle, mData->title, mData->title,
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, mSettings.width,
            mSettings.height, NULL, NULL, mData->hInstance, NULL);

        mData->hdc = GetDC(mData->hWnd);

        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL |
            PFD_DRAW_TO_WINDOW;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = mSettings.depth;
        pfd.cDepthBits = 32;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int nPixelFormat = ChoosePixelFormat(mData->hdc, &pfd);
        if (nPixelFormat == 0)
        {
            cerr << "failed ChoosePixelFormat" << endl;
            mSettings.width = 0;
            mSettings.height = 0;
            mSettings.depth = 0;
            return;
        }

        if (!SetPixelFormat(mData->hdc, nPixelFormat, &pfd))
        {
            cerr << "failed SetPixelFormat" << endl;
            mSettings.width = 0;
            mSettings.height = 0;
            mSettings.depth = 0;
            return;
        }

        HGLRC tempOpenGLEngine = wglCreateContext(mData->hdc);
        wglMakeCurrent(mData->hdc, tempOpenGLEngine);

        GLenum e = glewInit();
        if (e != GLEW_OK)
        {
            cerr << "failed to init GLEW" << endl;
            return;
        }

        if (!mSettings.context.vMajor)
        {
            mSettings.context.vMajor = 3;
            mSettings.context.vMinor = 3;
        }

        int attributes[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, mSettings.context.vMajor,
            WGL_CONTEXT_MINOR_VERSION_ARB, mSettings.context.vMinor,
            //WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            0
        };

        if (!mSettings.legacyContext
            && wglewIsSupported("WGL_ARB_create_context") == 1
            && wglCreateContextAttribsARB)
        {
            mData->hrc = wglCreateContextAttribsARB(mData->hdc, NULL,
                attributes);
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(tempOpenGLEngine);
            wglMakeCurrent(mData->hdc, mData->hrc);
        }
        else
        {
            mData->hrc = tempOpenGLEngine;
            mSettings.legacyContext = true;
        }

        ShowWindow(mData->hWnd, SW_SHOW);
        UpdateWindow(mData->hWnd);

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
    }

    void Engine::end()
    {
        if (mData->active)
        {
            wglMakeCurrent(mData->hdc, 0);
            wglDeleteContext(mData->hrc);
            ReleaseDC(mData->hWnd, mData->hdc);
            DestroyWindow(mData->hWnd);
            mSettings.width = 0;
            mSettings.height = 0;
            mSettings.depth = 0;
            mData->active = false;
            //PostQuitMessage(0);
        }

        delete mData;
    }

    void Engine::swapBuffers()
    {
        SwapBuffers(mData->hdc);
    }

    bool Engine::getEvent(Event& inEvent)
    {
        //cout << "dispatchEvents" << endl;
        MSG msg;
        if (!PeekMessage(&msg, mData->hWnd, 0, 0, PM_REMOVE)) return false;
        WPARAM wparam = msg.wParam;
        LPARAM lparam = msg.lParam;

        switch (msg.message)
        {
            case WM_MOUSEMOVE:
            {
                // http://msdn.microsoft.com/en-us/library/ms632654%28v=VS.85%29.aspx
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::Motion;
                inEvent.mouse.x = GET_X_LPARAM(lparam);
                inEvent.mouse.y = GET_Y_LPARAM(lparam);
                break;
            }

            case WM_MBUTTONDOWN:
            {
                //cout << "mouse MButton down" << endl;
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::ButtonDown;
                inEvent.mouse.button = MouseEvent::MiddleButton;
                break;
            }

            case WM_MBUTTONUP:
            {
                //cout << "mouse MButton up" << endl;
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::ButtonUp;
                inEvent.mouse.button = MouseEvent::MiddleButton;
                break;
            }

            case WM_LBUTTONDOWN:
            {
                //cout << "mouse LButton down" << endl;
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::ButtonDown;
                inEvent.mouse.button = MouseEvent::LeftButton;
                break;
            }

            case WM_LBUTTONUP:
            {
                //cout << "mouse LButton up" << endl;
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::ButtonUp;
                inEvent.mouse.button = MouseEvent::LeftButton;
                break;
            }

            case WM_RBUTTONDOWN:
            {
                //cout << "mouse RButton down" << endl;
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::ButtonDown;
                inEvent.mouse.button = MouseEvent::RightButton;
                break;
            }

            case WM_RBUTTONUP:
            {
                //cout << "mouse RButton up" << endl;
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::ButtonUp;
                inEvent.mouse.button = MouseEvent::RightButton;
                break;
            }

            case WM_MOUSELEAVE:
            {
                //cout << "mouse leave" << endl;
                inEvent.type = Event::Mouse;
                inEvent.mouse.event = MouseEvent::LeaveWindow;
                break;
            }

            case WM_MOUSEWHEEL:
            {
                //http://msdn.microsoft.com/en-us/library/ms645617%28v=VS.85%29.aspx
                //cout << "mouse wheel: " << GET_WHEEL_DELTA_WPARAM(wparam)
                //    << endl;
                inEvent.type = Event::Mouse;
                short w = GET_WHEEL_DELTA_WPARAM(wparam);
                if (w > 0)
                    inEvent.mouse.event = MouseEvent::WheelUp;
                else if (w < 0)
                    inEvent.mouse.event = MouseEvent::WheelDown;
                break;
            }

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            {
                // http://msdn.microsoft.com/en-us/library/ms646280%28VS.85%29.aspx
                // Bit 30 of lparam indicates last key state.

                unsigned int key = (lparam & 0x00ff0000) >> 16;
                bool extended = lparam & (1 << 24);
                inEvent.type = Event::Keyboard;
                inEvent.keyboard.event = lparam & 0x40000000 ?
                    KeyboardEvent::Repeat : KeyboardEvent::Press;
                inEvent.keyboard.key = lookupKey(key, extended);
                break;
            }

            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                unsigned int key = (lparam & 0x00ff0000) >> 16;
                bool extended = lparam & (1 << 24);
                inEvent.type = Event::Keyboard;
                inEvent.keyboard.event = KeyboardEvent::Release;
                inEvent.keyboard.key = lookupKey(key, extended);
                break;
            }

            // http://msdn.microsoft.com/en-us/library/ff468861%28v=VS.85%29.aspx
            case WM_SETFOCUS:
            {
                inEvent.type = Event::Window;
                inEvent.window.event = WindowEvent::Focus;
                break;
            }

            case WM_KILLFOCUS:
            {
                inEvent.type = Event::Window;
                inEvent.window.event = WindowEvent::Blur;
                break;
            }

            default:
            {
                activeEvent = &inEvent;
                activeWidth = &mSettings.width;
                activeHeight = &mSettings.height;

                TranslateMessage(&msg);
                DispatchMessage(&msg);

                activeEvent = NULL;
            }
        }

        return true;
    }

    void Engine::runModule(Module& inModule)
    {
        if (!mSettings.width) return;

        //mData->module = inModule;

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
        if (!inTitle || strlen(inTitle) > 254) return;

        strcpy(mData->title, inTitle);
        if (mSettings.width)
        {
            SetWindowText(mData->hWnd, mData->title);
            /// TODO: add error checking
        }
    }

    void Engine::setIconTitle(const char* inTitle)
    {
        /// unavailable in Win32 (?)
    }

    /// /// ///

    /// Only handles events that cannot be caught by PeekMessage. This
    /// generally covers window events.
    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (!activeEvent)
            return DefWindowProc(hWnd, msg, wparam, lparam);

        switch (msg)
        {
            case WM_SIZE:
            {
                *activeWidth = LOWORD(lparam);
                *activeHeight = HIWORD(lparam);

                activeEvent->type = Event::Window;
                activeEvent->window.event = WindowEvent::Resize;
                activeEvent->window.width = *activeWidth;
                activeEvent->window.height = *activeHeight;

                // http://msdn.microsoft.com/en-us/library/ms632646%28v=VS.85%29.aspx
                switch (wparam)
                {
                    case SIZE_MAXIMIZED:
                    {
                        //cout << "maximize" << endl;
                        activeEvent->window.resize = WindowEvent::Maximize;
                        break;
                    }

                    case SIZE_MINIMIZED:
                    {
                        //cout << "minimize" << endl;
                        activeEvent->window.resize = WindowEvent::Minimize;
                        break;
                    }

                    case SIZE_RESTORED:
                    {
                        //cout << "restore" << endl;
                        break;
                    }

                    default:
                    {}
                }

                return 0;
                break;
            }

            case WM_CREATE:
            {
                break;
            }

            case WM_CLOSE:
            {
                activeEvent->type = Event::Window;
                activeEvent->window.event = WindowEvent::Exit;
                break;
            }

            default:
                return DefWindowProc(hWnd, msg, wparam, lparam);
        }
        return 0;
    }
}
