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
    static int16u* activeWidth;
    static int16u* activeHeight;
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

        if (!mSettings.legacyContext && wglewIsSupported("WGL_ARB_create_context") == 1)
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
                inEvent.type = Event::MOUSE;
                inEvent.mouse.event = MouseEvent::MOTION;
                inEvent.mouse.x = GET_X_LPARAM(lparam);
                inEvent.mouse.y = GET_Y_LPARAM(lparam);
                break;
            }

            case WM_MBUTTONDOWN:
            {
                //cout << "mouse MButton down" << endl;
                inEvent.type = Event::MOUSE;
                inEvent.mouse.event = MouseEvent::BUTTON_DOWN;
                inEvent.mouse.button = MouseEvent::MIDDLE_BUTTON;
                break;
            }

            case WM_MBUTTONUP:
            {
                //cout << "mouse MButton up" << endl;
                inEvent.type = Event::MOUSE;
                inEvent.mouse.event = MouseEvent::BUTTON_UP;
                inEvent.mouse.button = MouseEvent::MIDDLE_BUTTON;
                break;
            }

            case WM_LBUTTONDOWN:
            {
                //cout << "mouse LButton down" << endl;
                inEvent.type = Event::MOUSE;
                inEvent.mouse.event = MouseEvent::BUTTON_DOWN;
                inEvent.mouse.button = MouseEvent::LEFT_BUTTON;
                break;
            }

            case WM_LBUTTONUP:
            {
                //cout << "mouse LButton up" << endl;
                inEvent.type = Event::MOUSE;
                inEvent.mouse.event = MouseEvent::BUTTON_UP;
                inEvent.mouse.button = MouseEvent::LEFT_BUTTON;
                break;
            }

            case WM_RBUTTONDOWN:
            {
                //cout << "mouse RButton down" << endl;
                inEvent.type = Event::MOUSE;
                inEvent.mouse.event = MouseEvent::BUTTON_DOWN;
                inEvent.mouse.button = MouseEvent::RIGHT_BUTTON;
                break;
            }

            case WM_RBUTTONUP:
            {
                //cout << "mouse RButton up" << endl;
                inEvent.type = Event::MOUSE;
                inEvent.mouse.event = MouseEvent::BUTTON_UP;
                inEvent.mouse.button = MouseEvent::RIGHT_BUTTON;
                break;
            }

            case WM_MOUSELEAVE:
            {
                //cout << "mouse leave" << endl;
                inEvent.type = Event::MOUSE;
                inEvent.mouse.event = MouseEvent::LEAVE_WINDOW;
                break;
            }

            case WM_MOUSEWHEEL:
            {
                //http://msdn.microsoft.com/en-us/library/ms645617%28v=VS.85%29.aspx
                //cout << "mouse wheel: " << GET_WHEEL_DELTA_WPARAM(wparam)
                //    << endl;
                inEvent.type = Event::MOUSE;
                short w = GET_WHEEL_DELTA_WPARAM(wparam);
                if (w > 0)
                    inEvent.mouse.event = MouseEvent::WHEEL_UP;
                else if (w < 0)
                    inEvent.mouse.event = MouseEvent::WHEEL_DOWN;
                break;
            }

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            {
                // http://msdn.microsoft.com/en-us/library/ms646280%28VS.85%29.aspx
                // Bit 30 of lparam indicates last key state.

                unsigned int key = (lparam & 0x00ff0000) >> 16;
                bool extended = lparam & (1 << 24);
                inEvent.type = Event::KEYBOARD;
                inEvent.keyboard.event = lparam & 0x40000000 ?
                    KeyboardEvent::REPEAT : KeyboardEvent::PRESS;
                inEvent.keyboard.key = convertToKeyCode(key, extended);
                break;
            }

            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                unsigned int key = (lparam & 0x00ff0000) >> 16;
                bool extended = lparam & (1 << 24);
                inEvent.type = Event::KEYBOARD;
                inEvent.keyboard.event = KeyboardEvent::RELEASE;
                inEvent.keyboard.key = convertToKeyCode(key, extended);
                break;
            }

            // http://msdn.microsoft.com/en-us/library/ff468861%28v=VS.85%29.aspx
            case WM_SETFOCUS:
            {
                inEvent.type = Event::WINDOW;
                inEvent.window.event = WindowEvent::FOCUS;
                break;
            }

            case WM_KILLFOCUS:
            {
                inEvent.type = Event::WINDOW;
                inEvent.window.event = WindowEvent::BLUR;
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
        event.type = Event::WINDOW;
        event.window.event = WindowEvent::RESIZE;
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

    Key::Code convertToKeyCode(unsigned int inCode, bool inExtended)
    {
        switch (inCode)
        {
            case 1: return Key::ESCAPE;
            case 2: return Key::TR1;
            case 3: return Key::TR2;
            case 4: return Key::TR3;
            case 5: return Key::TR4;
            case 6: return Key::TR5;
            case 7: return Key::TR6;
            case 8: return Key::TR7;
            case 9: return Key::TR8;
            case 10: return Key::TR9;
            case 11: return Key::TR0;
            case 12: return Key::MINUS;
            case 13: return Key::EQUALS;
            case 14: return Key::BACKSPACE;
            case 15: return Key::TAB;
            case 16: return Key::Q;
            case 17: return Key::W;
            case 18: return Key::E;
            case 19: return Key::R;
            case 20: return Key::T;
            case 21: return Key::Y;
            case 22: return Key::U;
            case 23: return Key::I;
            case 24: return Key::O;
            case 25: return Key::P;
            case 26: return Key::LEFT_BRACKET;
            case 27: return Key::RIGHT_BRACKET;
            case 28: return inExtended ? Key::KP_ENTER : Key::ENTER;
            case 29: return inExtended ? Key::RIGHT_CONTROL : Key::LEFT_CONTROL;
            case 30: return Key::A;
            case 31: return Key::S;
            case 32: return Key::D;
            case 33: return Key::F;
            case 34: return Key::G;
            case 35: return Key::H;
            case 36: return Key::J;
            case 37: return Key::K;
            case 38: return Key::L;
            case 39: return Key::SEMICOLON;
            case 40: return Key::QUOTE;
            case 41: return Key::BACK_QUOTE;
            case 42: return Key::LEFT_SHIFT;
            case 43: return Key::BACKSLASH;
            case 44: return Key::Z;
            case 45: return Key::X;
            case 46: return Key::C;
            case 47: return Key::V;
            case 48: return Key::B;
            case 49: return Key::N;
            case 50: return Key::M;
            case 51: return Key::COMMA;
            case 52: return Key::PERIOD;
            case 53: return inExtended ? Key::KP_SLASH : Key::SLASH;
            case 54: return Key::RIGHT_SHIFT;
            case 55: return Key::KP_ASTERISK;
            case 56: return inExtended ? Key::RIGHT_ALT : Key::LEFT_ALT;
            case 57: return Key::SPACE;
            case 58: return Key::CAPSLOCK;
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
            case 69: return inExtended ? Key::NUMLOCK : Key::PAUSE;
            case 70: return Key::SCROLL_LOCK;
            case 71: return inExtended ? Key::HOME : Key::KP7;
            case 72: return inExtended ? Key::UP : Key::KP8;
            case 73: return inExtended ? Key::PAGE_UP : Key::KP9;
            case 74: return Key::KP_SLASH;
            case 75: return inExtended ? Key::LEFT : Key::KP4;
            case 76: return Key::KP5;
            case 77: return inExtended ? Key::RIGHT : Key::KP6;
            case 78: return Key::KP_PLUS;
            case 79: return inExtended ? Key::END : Key::KP1;
            case 80: return inExtended ? Key::DOWN : Key::KP2;
            case 81: return inExtended ? Key::PAGE_DOWN : Key::KP3;
            case 82: return inExtended ? Key::INSERT : Key::KP0;
            case 83: return inExtended ? Key::DEL : Key::KP_PERIOD;

            case 87: return Key::F11;
            case 88: return Key::F12;

            case 91: return Key::LEFT_SUPER; // extended
            case 92: return Key::RIGHT_SUPER; // extended
            //case 93: //menu key // extended

            default: return Key::UNKNOWN;
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

                activeEvent->type = Event::WINDOW;
                activeEvent->window.event = WindowEvent::RESIZE;
                activeEvent->window.width = *activeWidth;
                activeEvent->window.height = *activeHeight;

                // http://msdn.microsoft.com/en-us/library/ms632646%28v=VS.85%29.aspx
                switch (wparam)
                {
                    case SIZE_MAXIMIZED:
                    {
                        //cout << "maximize" << endl;
                        activeEvent->window.resize = WindowEvent::MAXIMIZE;
                        break;
                    }

                    case SIZE_MINIMIZED:
                    {
                        //cout << "minimize" << endl;
                        activeEvent->window.resize = WindowEvent::MINIMIZE;
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
                activeEvent->type = Event::WINDOW;
                activeEvent->window.event = WindowEvent::EXIT;
                break;
            }

            default:
                return DefWindowProc(hWnd, msg, wparam, lparam);
        }
        return 0;
    }
}
