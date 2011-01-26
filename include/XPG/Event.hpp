#ifndef XPGH_EVENT
#define XPGH_EVENT

#include "DataTypes.hpp"

namespace XPG
{
    namespace Key
    {
        enum Code
        {
            UNKNOWN,

            /// letter keys
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W,
            X, Y, Z,

            /// function keys
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

            /// top row numbers/symbols
            ESCAPE, BACK_QUOTE, TR1, TR2, TR3, TR4, TR5, TR6, TR7, TR8, TR9,
            TR0, MINUS, EQUALS, BACKSPACE,

            /// symbols surrounding letter keys
            TAB, LEFT_BRACKET, RIGHT_BRACKET, BACKSLASH, CAPSLOCK, SEMICOLON,
            QUOTE, ENTER, COMMA, PERIOD, SLASH, SPACE,

            /// mod keys
            LEFT_SHIFT, RIGHT_SHIFT, LEFT_CONTROL, RIGHT_CONTROL, LEFT_ALT,
            RIGHT_ALT, LEFT_SUPER, RIGHT_SUPER,

            /// control keys
            INSERT, HOME, PAGE_UP, PAGE_DOWN, END, DEL, UP, DOWN, LEFT,
            RIGHT, SCROLL_LOCK, PAUSE, PRINT_SCREEN,

            /// keypad
            KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9, KP_PERIOD,
            KP_ENTER, KP_PLUS, KP_MINUS, KP_ASTERISK, KP_SLASH, NUMLOCK
        };
    }

    struct WindowEvent
    {
        enum { FOCUS, BLUR, RESIZE, EXIT } event;
        enum { MINIMIZE, MAXIMIZE, RESTORE } resize;
        int32u width;
        int32u height;
    };

    struct MouseEvent
    {
        enum { MOTION, BUTTON_DOWN, BUTTON_UP, WHEEL_DOWN, WHEEL_UP,
            LEAVE_WINDOW, ENTER_WINDOW } event;
        enum { LEFT_BUTTON = 1, RIGHT_BUTTON = 2, MIDDLE_BUTTON = 3 };
        int8u button;
        int32u x;
        int32u y;
    };

    struct JoystickEvent
    {
        enum { AXIS, BUTTON_DOWN, BUTTON_UP, HAT, BALL } event;
        int8u button;
        int32s x;
        int32s y;
    };

    struct KeyboardEvent
    {
        enum { PRESS, REPEAT, RELEASE } event;
        Key::Code key;
    };

    struct Event
    {
        enum { NONE, WINDOW, MOUSE, JOYSTICK, KEYBOARD } type;
        WindowEvent window;
        MouseEvent mouse;
        JoystickEvent joystick;
        KeyboardEvent keyboard;
    };
}

#endif
