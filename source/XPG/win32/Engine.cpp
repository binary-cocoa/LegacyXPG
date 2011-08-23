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
    Key::Code convertToKeyCode(unsigned int inCode, bool inExtended);
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
                inEvent.keyboard.key = convertToKeyCode(key, extended);
                break;
            }

            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                unsigned int key = (lparam & 0x00ff0000) >> 16;
                bool extended = lparam & (1 << 24);
                inEvent.type = Event::Keyboard;
                inEvent.keyboard.event = KeyboardEvent::Release;
                inEvent.keyboard.key = convertToKeyCode(key, extended);
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

    /// TODO: Convert this function into a LUT.
    Key::Code convertToKeyCode(unsigned int inCode, bool inExtended)
    {
        switch (inCode)
        {
            case 1: return Key::Escape;
            case 2: return Key::TopRow1;
            case 3: return Key::TopRow2;
            case 4: return Key::TopRow3;
            case 5: return Key::TopRow4;
            case 6: return Key::TopRow5;
            case 7: return Key::TopRow6;
            case 8: return Key::TopRow7;
            case 9: return Key::TopRow8;
            case 10: return Key::TopRow9;
            case 11: return Key::TopRow0;
            case 12: return Key::Minus;
            case 13: return Key::Equals;
            case 14: return Key::Backspace;
            case 15: return Key::Tab;
            case 16: return Key::LetterQ;
            case 17: return Key::LetterW;
            case 18: return Key::LetterE;
            case 19: return Key::LetterR;
            case 20: return Key::LetterT;
            case 21: return Key::LetterY;
            case 22: return Key::LetterU;
            case 23: return Key::LetterI;
            case 24: return Key::LetterO;
            case 25: return Key::LetterP;
            case 26: return Key::LeftBracket;
            case 27: return Key::RightBracket;
            case 28: return inExtended ? Key::KeypadEnter : Key::Enter;
            case 29: return inExtended ? Key::RightControl : Key::LeftControl;
            case 30: return Key::LetterA;
            case 31: return Key::LetterS;
            case 32: return Key::LetterD;
            case 33: return Key::LetterF;
            case 34: return Key::LetterG;
            case 35: return Key::LetterH;
            case 36: return Key::LetterJ;
            case 37: return Key::LetterK;
            case 38: return Key::LetterL;
            case 39: return Key::Semicolon;
            case 40: return Key::Quote;
            case 41: return Key::BackQuote;
            case 42: return Key::LeftShift;
            case 43: return Key::Backslash;
            case 44: return Key::LetterZ;
            case 45: return Key::LetterX;
            case 46: return Key::LetterC;
            case 47: return Key::LetterV;
            case 48: return Key::LetterB;
            case 49: return Key::LetterN;
            case 50: return Key::LetterM;
            case 51: return Key::Comma;
            case 52: return Key::Period;
            case 53: return inExtended ? Key::KeypadSlash : Key::Slash;
            case 54: return Key::RightShift;
            case 55: return Key::KeypadAsterisk;
            case 56: return inExtended ? Key::RightAlt : Key::LeftAlt;
            case 57: return Key::Space;
            case 58: return Key::CapsLock;
            case 59: return Key::F1;
            case 60: return Key::F2;
            case 61: return Key::F3;
            case 62: return Key::F4;
            case 63: return Key::F5;
            case 64: return Key::F6;
            case 65: return Key::F7;
            case 66: return Key::F8;
            case 67: return Key::F9;
            case 68: return Key::F10;
            case 69: return inExtended ? Key::NumLock : Key::Pause;
            case 70: return Key::ScrollLock;
            case 71: return inExtended ? Key::Home : Key::Keypad7;
            case 72: return inExtended ? Key::Up : Key::Keypad8;
            case 73: return inExtended ? Key::PageUp : Key::Keypad9;
            case 74: return Key::KeypadSlash;
            case 75: return inExtended ? Key::Left : Key::Keypad4;
            case 76: return Key::Keypad5;
            case 77: return inExtended ? Key::Right : Key::Keypad6;
            case 78: return Key::KeypadPlus;
            case 79: return inExtended ? Key::End : Key::Keypad1;
            case 80: return inExtended ? Key::Down : Key::Keypad2;
            case 81: return inExtended ? Key::PageDown : Key::Keypad3;
            case 82: return inExtended ? Key::Insert : Key::Keypad0;
            case 83: return inExtended ? Key::Delete : Key::KeypadPeriod;

            case 87: return Key::F11;
            case 88: return Key::F12;

            case 91: return Key::LeftSuper; // extended
            case 92: return Key::RightSuper; // extended
            //case 93: //menu key // extended

            default: return Key::Unknown;
        }
    }

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
