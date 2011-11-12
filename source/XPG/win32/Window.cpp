#include "Window.hpp"

#include <cstdlib>
#include <climits>
#include <iostream>
using namespace std;

namespace XPG
{
    const char* const ClassName = "XPG";

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

    // evil globals (working on a way to avoid needing these)
    static uint16* activeWidth;
    static uint16* activeHeight;
    static Event* activeEvent = NULL;

    bool Window::mDidSetupGlew = false;

    void Window::setupExtensions()
    {
        if (!mDidSetupGlew)
        {
            GLenum e = glewInit();

            if (e != GLEW_OK)
            {
                cerr << "failed to setup GLEW\n";
            }

            mDidSetupGlew = true;
        }
    }

    Window::Window(HINSTANCE inINSTANCE) : mWND(0), mDC(0), mRC(0),
    mINSTANCE(inINSTANCE), mStyle(0), mExStyle(0)
    {
        int pixelFormat;
        int fullWidth;
        int fullHeight;

        RECT wa;
        POINT pos;

        mStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE;
        mExStyle = WS_EX_APPWINDOW;

        if (false) // full screen check
        {
        }
        else
        {
            mStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

            // resize flags
        }

        getFullWindowSize(640, 480, &fullWidth, &fullHeight);

        if (false) // full screen check
        {
        }
        else
        {
            SystemParametersInfo(SPI_GETWORKAREA, 0, &wa, 0);
        }

        mWND = CreateWindowEx(mExStyle, ClassName, "XPG OpenGL", mStyle,
            wa.left, wa.top, fullWidth, fullHeight, NULL, NULL, mINSTANCE, NULL);

        if (!mWND)
        {
            cerr << "unable to create window\n";
            exit(1);
        }

        mDC = GetDC(mWND);
        if (!mDC)
        {
            cerr << "unable to retrieve DC\n";
            exit(1);
        }

        FrameBufferConfig fbc;
        memset(&fbc, 0, sizeof(FrameBufferConfig));
        pixelFormat = choosePixelFormat(&fbc);
        if (!pixelFormat)
        {
            cerr << "failed to choose a pixel format\n";
            exit(1);
        }

        if (!createContext(pixelFormat))
        {
            cerr << "failed to create context\n";
            exit(1);
        }

        if (!wglMakeCurrent(mDC, mRC))
        {
            cerr << "failed on wglMakeCurrent\n";
            exit(1);
        }

        setupExtensions();

        GetCursorPos(&pos);
        ScreenToClient(mWND, &pos);

        BringWindowToTop(mWND);
        SetForegroundWindow(mWND);
        SetFocus(mWND);

        ShowWindow(mWND, SW_SHOW);
    }

    Window::~Window()
    {
        if (mRC)
        {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(mRC);
            mRC = 0;
        }

        if (mDC)
        {
            ReleaseDC(mWND, mDC);
            mDC = 0;
        }

        if (mWND)
        {
            DestroyWindow(mWND);
            mWND = NULL;
        }
    }

    bool Window::getEvent(Event& inEvent)
    {
        inEvent.type = Event::NoEvent;

        if (mDispatchResize)
        {
            mDispatchResize = false;
            inEvent.type = Event::Window;
            inEvent.window.event = WindowEvent::Resize;
            inEvent.window.resize = WindowEvent::Regular;

            RECT rc;
            GetWindowRect(mWND, &rc);
            mWidth = rc.right - rc.left;
            mHeight = rc.bottom - rc.top;

            inEvent.window.width = mWidth;
            inEvent.window.height = mHeight;
            return true;
        }

        MSG msg;
        if (!PeekMessage(&msg, mWND, 0, 0, PM_REMOVE)) return false;
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
                activeWidth = &mWidth;
                activeHeight = &mHeight;

                TranslateMessage(&msg);
                DispatchMessage(&msg);

                activeEvent = NULL;
            }
        }

        return true;
    }

    bool Window::createContext(int inPixelFormat)
    {
        PIXELFORMATDESCRIPTOR pfd;
        if (!DescribePixelFormat(mDC, inPixelFormat, sizeof(pfd), &pfd))
        {
            cerr << "failed DescribePixelFormat\n";
            return false;
        }

        if (!SetPixelFormat(mDC, inPixelFormat, &pfd))
        {
            cerr << "failed SetPixelFormat\n";
            return false;
        }

        if (wglCreateContextAttribsARB)
        {
            int attributes[64];
            memset(attributes, 0, sizeof(attributes));

            int* a = attributes;

            *a++ = WGL_CONTEXT_MAJOR_VERSION_ARB;
            *a++ = 3;
            *a++ = WGL_CONTEXT_MINOR_VERSION_ARB;
            *a++ = 2;
            *a++ = WGL_CONTEXT_FLAGS_ARB;
            *a++ = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
            *a++ = WGL_CONTEXT_PROFILE_MASK_ARB;
            *a++ = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;

            mRC = wglCreateContextAttribsARB(mDC, NULL, attributes);

            if (!mRC)
            {
                cerr << "failed on wglCreateContextAttribsARB\n";
                return false;
            }
        }
        else
        {
            mRC = wglCreateContext(mDC);

            if (!mRC)
            {
                cerr << "failed wglCreateContext\n";
                return false;
            }
        }

        return true;
    }

    void Window::getFullWindowSize(int clientWidth, int clientHeight,
        int* fullWidth, int* fullHeight)
    {
        RECT rect;

        rect.left = (long)0;
        rect.right = (long)clientWidth - 1;
        rect.top = (long)0;
        rect.bottom = (long)clientHeight - 1;

        AdjustWindowRectEx(&rect, mStyle, FALSE, mExStyle);

        *fullWidth = rect.right - rect.left + 1;
        *fullHeight = rect.bottom - rect.top + 1;
    }

    int Window::choosePixelFormat(const FrameBufferConfig* inConfig)
    {
        unsigned int configCount = 0;
        int outFormat = 0;
        FrameBufferConfig* configs = NULL;

        configs = getConfigs(&configCount);
        if (!configs)
        {
            cerr << "failed to find any usable frame buffer configs\n";
            return 0;
        }

        const FrameBufferConfig* closestConfig = chooseConfig(inConfig, configs,
            configCount);

        if (!closestConfig)
        {
            cerr << "failed to find the closest config\n";
            delete [] configs;
            return 0;
        }

        outFormat = (int)closestConfig->platformID;
        delete [] configs;
        return outFormat;
    }

    FrameBufferConfig* Window::getConfigs(unsigned int* outSize)
    {
        PIXELFORMATDESCRIPTOR pfd;

        *outSize = 0;

        int count = wglGetPixelFormatAttribivARB ?
            getPixelFormatAttrib(1, WGL_NUMBER_PIXEL_FORMATS_ARB) :
            DescribePixelFormat(mDC, 1, sizeof(PIXELFORMATDESCRIPTOR), NULL);

        if (count < 1)
        {
            cerr << "no win32 pixel formats available\n";
            exit(1);
            return NULL;
        }

        FrameBufferConfig* outResult = new FrameBufferConfig[count];

        for (int i = 1; i <= count; ++i)
        {
            if (wglGetPixelFormatAttribivARB)
            {
                if (!getPixelFormatAttrib(i, WGL_SUPPORT_OPENGL_ARB)
                    || !getPixelFormatAttrib(i, WGL_DRAW_TO_WINDOW_ARB)
                    || !getPixelFormatAttrib(i, WGL_DOUBLE_BUFFER_ARB)
                    || getPixelFormatAttrib(i, WGL_PIXEL_TYPE_ARB)
                        != WGL_TYPE_RGBA_ARB
                    || getPixelFormatAttrib(i, WGL_ACCELERATION_ARB)
                        == WGL_NO_ACCELERATION_ARB
                    )
                {
                    continue;
                }

                outResult[*outSize].redBits =
                    getPixelFormatAttrib(i, WGL_RED_BITS_ARB);
                outResult[*outSize].greenBits =
                    getPixelFormatAttrib(i, WGL_GREEN_BITS_ARB);
                outResult[*outSize].blueBits =
                    getPixelFormatAttrib(i, WGL_BLUE_BITS_ARB);
                outResult[*outSize].alphaBits =
                    getPixelFormatAttrib(i, WGL_ALPHA_BITS_ARB);
                outResult[*outSize].depthBits =
                    getPixelFormatAttrib(i, WGL_DEPTH_BITS_ARB);
                outResult[*outSize].stencilBits =
                    getPixelFormatAttrib(i, WGL_STENCIL_BITS_ARB);
                outResult[*outSize].accumRedBits =
                    getPixelFormatAttrib(i, WGL_ACCUM_RED_BITS_ARB);
                outResult[*outSize].accumGreenBits =
                    getPixelFormatAttrib(i, WGL_ACCUM_GREEN_BITS_ARB);
                outResult[*outSize].accumBlueBits =
                    getPixelFormatAttrib(i, WGL_ACCUM_BLUE_BITS_ARB);
                outResult[*outSize].accumAlphaBits =
                    getPixelFormatAttrib(i, WGL_ACCUM_ALPHA_BITS_ARB);
                outResult[*outSize].auxBuffers =
                    getPixelFormatAttrib(i, WGL_AUX_BUFFERS_ARB);
                outResult[*outSize].stereo =
                    getPixelFormatAttrib(i, WGL_STEREO_ARB);
                outResult[*outSize].samples = 0;

                if (glewIsExtensionSupported("WGL_ARB_multisample"))
                {
                    outResult[*outSize].samples =
                        getPixelFormatAttrib(i, WGL_SAMPLES_ARB);
                }
            }
            else
            {
                if (!DescribePixelFormat(mDC, i, sizeof(PIXELFORMATDESCRIPTOR),
                    &pfd))
                {
                    continue;
                }

                if (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW)
                    || !(pfd.dwFlags & PFD_SUPPORT_OPENGL)
                    || !(pfd.dwFlags & PFD_DOUBLEBUFFER))
                {
                    continue;
                }

                if (!(pfd.dwFlags & PFD_GENERIC_ACCELERATED)
                    && (pfd.dwFlags & PFD_GENERIC_FORMAT))
                {
                    continue;
                }

                if (pfd.iPixelType != PFD_TYPE_RGBA) continue;

                outResult[*outSize].redBits = pfd.cRedBits;
                outResult[*outSize].greenBits = pfd.cGreenBits;
                outResult[*outSize].blueBits = pfd.cBlueBits;
                outResult[*outSize].alphaBits = pfd.cAlphaBits;
                outResult[*outSize].depthBits = pfd.cDepthBits;
                outResult[*outSize].stencilBits = pfd.cStencilBits;
                outResult[*outSize].accumRedBits = pfd.cAccumRedBits;
                outResult[*outSize].accumGreenBits = pfd.cAccumGreenBits;
                outResult[*outSize].accumBlueBits = pfd.cAccumBlueBits;
                outResult[*outSize].accumAlphaBits = pfd.cAccumAlphaBits;
                outResult[*outSize].auxBuffers = pfd.cAuxBuffers;
                outResult[*outSize].stereo = (pfd.dwFlags & PFD_STEREO) ?
                    GL_TRUE : GL_FALSE;
                outResult[*outSize].samples = 0;
            }

            outResult[*outSize].platformID = i;
            ++(*outSize);
        }

        return outResult;
    }

    const FrameBufferConfig* Window::chooseConfig(
        const FrameBufferConfig* inConfig, const FrameBufferConfig* inConfigs,
        unsigned int inCount)
    {
        unsigned int missing, leastMissing = UINT_MAX;
        unsigned int colorDiff, leastColorDiff = UINT_MAX;
        unsigned int extraDiff, leastExtraDiff = UINT_MAX;
        const FrameBufferConfig* current = NULL;
        const FrameBufferConfig* closest = NULL;

        for (unsigned int i = 0; i < inCount; ++i)
        {
            current = inConfigs + i;

            if (inConfig->stereo > 0 && current->stereo == 0) continue;

            missing = 0;

            if (inConfig->alphaBits > 0 && current->alphaBits == 0) ++missing;
            if (inConfig->depthBits > 0 && current->depthBits == 0) ++missing;
            if (inConfig->stencilBits > 0 && current->stencilBits == 0)
                ++missing;

            if (inConfig->auxBuffers > 0
                && current->auxBuffers < inConfig->auxBuffers)
            {
                missing += inConfig->auxBuffers - current->auxBuffers;
            }

            if (inConfig->samples > 0 && current->samples == 0) ++missing;

            colorDiff = 0;

            if (inConfig->redBits > 0)
            {
                colorDiff += (inConfig->redBits - current->redBits) *
                             (inConfig->redBits - current->redBits);
            }

            if (inConfig->greenBits > 0)
            {
                colorDiff += (inConfig->greenBits - current->greenBits) *
                             (inConfig->greenBits - current->greenBits);
            }

            if (inConfig->blueBits > 0)
            {
                colorDiff += (inConfig->blueBits - current->blueBits) *
                             (inConfig->blueBits - current->blueBits);
            }

            extraDiff = 0;

            if (inConfig->alphaBits > 0)
            {
                extraDiff += (inConfig->alphaBits - current->alphaBits) *
                             (inConfig->alphaBits - current->alphaBits);
            }

            if (inConfig->depthBits > 0)
            {
                extraDiff += (inConfig->depthBits - current->depthBits) *
                             (inConfig->depthBits - current->depthBits);
            }

            if (inConfig->stencilBits > 0)
            {
                extraDiff += (inConfig->stencilBits - current->stencilBits) *
                             (inConfig->stencilBits - current->stencilBits);
            }

            if (inConfig->accumRedBits > 0)
            {
                extraDiff += (inConfig->accumRedBits - current->accumRedBits) *
                             (inConfig->accumRedBits - current->accumRedBits);
            }

            if (inConfig->accumGreenBits > 0)
            {
                extraDiff += (inConfig->accumGreenBits
                    - current->accumGreenBits) * (inConfig->accumGreenBits
                    - current->accumGreenBits);
            }

            if (inConfig->accumBlueBits > 0)
            {
                extraDiff += (inConfig->accumBlueBits
                    - current->accumBlueBits) * (inConfig->accumBlueBits
                    - current->accumBlueBits);
            }

            if (inConfig->accumAlphaBits > 0)
            {
                extraDiff += (inConfig->accumAlphaBits
                    - current->accumAlphaBits) * (inConfig->accumAlphaBits
                    - current->accumAlphaBits);
            }

            if (inConfig->samples > 0)
            {
                extraDiff += (inConfig->samples - current->samples) *
                             (inConfig->samples - current->samples);
            }

            if (missing < leastMissing)
            {
                closest = current;
            }
            else if (missing == leastMissing)
            {
                if ((colorDiff < leastColorDiff) ||
                    (colorDiff == leastColorDiff && extraDiff < leastExtraDiff))
                {
                    closest = current;
                }
            }

            if (current == closest)
            {
                leastMissing = missing;
                leastColorDiff = colorDiff;
                leastExtraDiff = extraDiff;
            }
        }

        return closest;
    }

    int Window::getPixelFormatAttrib(int inPixelFormat, int inAttrib)
    {
        int outValue = 0;

        if (!wglGetPixelFormatAttribivARB || !wglGetPixelFormatAttribivARB(mDC,
            inPixelFormat, 0, 1, &inAttrib, &outValue))
        {
            return 0;
        }

        return outValue;
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
                activeEvent->window.resize = WindowEvent::Regular;
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
                        activeEvent->window.resize = WindowEvent::Restore;
                        //activeSuccess = true;
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
