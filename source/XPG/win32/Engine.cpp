#include <XPG/Engine.hpp>
#include <XPG/Platforms.hpp>
#include <XPG/Timer.hpp>
#include <XPG/private/windows.hpp>
#include <XPG/private/glew.h>
#include <XPG/private/wglew.h>

#include "DirectInput.hpp"

#include <iostream>
using namespace std;

#define KEYBOARD_BUFFER_SIZE 128

namespace XPG
{
    Key::Code convertKeyCode(unsigned int inData);
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
        LPDIRECTINPUT8 di8;
        LPDIRECTINPUTDEVICE8 did8;
    };

    void Engine::begin()
    {
        mData = new PrivateData;
        mData->active = false;
        strcpy(mData->title, "OpenGL 3");

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

        // http://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.reference.directinput8create%28v=VS.85%29.aspx
        HRESULT result = DirectInput8Create(mData->hInstance,
            DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mData->di8,
            NULL);

        if (result != DI_OK)
        {
            // TODO: handle DirectInput error
            cerr << "DirectInput error -- ";

            switch (result)
            {
                case DIERR_BETADIRECTINPUTVERSION:
                    cerr << "DIERR_BETADIRECTINPUTVERSION\n";
                    break;

                case DIERR_INVALIDPARAM:
                    cerr << "DIERR_INVALIDPARAM\n";
                    break;

                case DIERR_OLDDIRECTINPUTVERSION:
                    cerr << "DIERR_OLDDIRECTINPUTVERSION\n";
                    break;

                case DIERR_OUTOFMEMORY:
                    cerr << "DIERR_OUTOFMEMORY\n";
                    break;

                default:
                    cerr << "other?\n";
            }

            mData->did8 = NULL;
        }
        else
        {
            cerr << "DirectInput OK\n";

            mData->di8->CreateDevice(GUID_SysKeyboard, &mData->did8, NULL);
            if (mData->did8->SetDataFormat(&c_dfDIKeyboard) != DI_OK) LDB;
            if (mData->did8->SetCooperativeLevel(mData->hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK) LDB;
            DIPROPDWORD dipdw;
            dipdw.diph.dwSize = sizeof(DIPROPDWORD);
            dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
            dipdw.diph.dwObj = 0;
            dipdw.diph.dwHow = DIPH_DEVICE;
            dipdw.dwData = KEYBOARD_BUFFER_SIZE;

            mData->did8->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
            mData->did8->Acquire();
        }
    }

    void Engine::end()
    {
        if (mData->active)
        {
            if (mData->di8)
            {
                mData->di8->Release();
                mData->did8->Unacquire();
                mData->did8->Release();
            }

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
        char keyboard[256];
        bool keyevent[256];

        mData->did8->GetDeviceState(sizeof(keyboard), (LPVOID)keyboard);

        DIDEVICEOBJECTDATA key_pressed[KEYBOARD_BUFFER_SIZE];
        DWORD dwItems;
        HRESULT hRes = DIERR_INPUTLOST;

        while (DI_OK != hRes)
        {
            dwItems = KEYBOARD_BUFFER_SIZE;
            hRes = mData->did8->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),
                key_pressed, &dwItems, 0);

            if (hRes != DI_OK) mData->did8->Acquire();
        }

        if (key_pressed[0].dwData & 0x80)
            cout << key_pressed[0].dwOfs << endl;

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
            {
                // http://msdn.microsoft.com/en-us/library/ms646280%28VS.85%29.aspx
                // Bit 30 of lparam indicates last key state.
                // We do not want to repeat this event.
                if (lparam & 0x40000000) break;

                unsigned int key = (unsigned int)wparam;
                //cout << "key down -- " << key << endl;
                inEvent.type = Event::KEYBOARD;
                inEvent.keyboard.event = KeyboardEvent::PRESS;
                inEvent.keyboard.key = convertKeyCode(key);
                break;
            }

            case WM_KEYUP:
            {
                unsigned int key = (unsigned int)wparam;
                //cout << "key up -- " << key << endl;
                inEvent.type = Event::KEYBOARD;
                inEvent.keyboard.event = KeyboardEvent::RELEASE;
                inEvent.keyboard.key = convertKeyCode(key);
                break;
            }

            // http://msdn.microsoft.com/en-us/library/ff468861%28v=VS.85%29.aspx
            case WM_SETFOCUS:
            {
                //cout << "focus" << endl;
                inEvent.type = Event::WINDOW;
                inEvent.window.event = WindowEvent::FOCUS;
                break;
            }

            case WM_KILLFOCUS:
            {
                //cout << "blur" << endl;
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

    Key::Code convertKeyCode(unsigned int inData)
    {
        switch (inData)
        {
            case 65: return Key::A;
            case 66: return Key::B;
            case 67: return Key::C;
            case 68: return Key::D;
            case 69: return Key::E;
            case 70: return Key::F;
            case 71: return Key::G;
            case 72: return Key::H;
            case 73: return Key::I;
            case 74: return Key::J;
            case 75: return Key::K;
            case 76: return Key::L;
            case 77: return Key::M;
            case 78: return Key::N;
            case 79: return Key::O;
            case 80: return Key::P;
            case 81: return Key::Q;
            case 82: return Key::R;
            case 83: return Key::S;
            case 84: return Key::T;
            case 85: return Key::U;
            case 86: return Key::V;
            case 87: return Key::W;
            case 88: return Key::X;
            case 89: return Key::Y;
            case 90: return Key::Z;

            case 48: return Key::TR0;
            case 49: return Key::TR1;
            case 50: return Key::TR2;
            case 51: return Key::TR3;
            case 52: return Key::TR4;
            case 53: return Key::TR5;
            case 54: return Key::TR6;
            case 55: return Key::TR7;
            case 56: return Key::TR8;
            case 57: return Key::TR9;

            case 27: return Key::ESCAPE;
            case 192: return Key::BACK_QUOTE;
            case 189: return Key::MINUS;
            case 187: return Key::EQUALS;
            case 8: return Key::BACKSPACE;
            case 9: return Key::TAB;
            case 219: return Key::LEFT_BRACKET;
            case 221: return Key::RIGHT_BRACKET;
            case 220: return Key::BACKSLASH;
            case 20: return Key::CAPSLOCK;
            case 186: return Key::SEMICOLON;
            case 222: return Key::QUOTE;
            case 13: return Key::ENTER;
            case 188: return Key::COMMA;
            case 190: return Key::PERIOD;
            case 191: return Key::SLASH;

            case 45: return Key::INSERT;
            case 46: return Key::DEL;
            case 36: return Key::HOME;
            case 35: return Key::END;
            case 33: return Key::PAGE_UP;
            case 34: return Key::PAGE_DOWN;
            case 37: return Key::LEFT;
            case 39: return Key::RIGHT;
            case 40: return Key::DOWN;
            case 38: return Key::UP;

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
