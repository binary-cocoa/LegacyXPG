#ifndef XPGH_WINDOW
#define XPGH_WINDOW

#include <XPG/private/windows.hpp>
#include <XPG/private/glew.h>
#include <XPG/private/wglew.h>

#include <XPG/Event.hpp>

namespace XPG
{
    extern const char* const ClassName;

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

    struct FrameBufferConfig
    {
        int         redBits;
        int         greenBits;
        int         blueBits;
        int         alphaBits;
        int         depthBits;
        int         stencilBits;
        int         accumRedBits;
        int         accumGreenBits;
        int         accumBlueBits;
        int         accumAlphaBits;
        int         auxBuffers;
        int         stereo;
        int         samples;
        INT_PTR  platformID;
    };

    class Window
    {
        public:
            Window(HINSTANCE inINSTANCE);
            ~Window();

            inline void swapBuffers()
            {
                SwapBuffers(mDC);
            }

            bool getEvent(Event& inEvent);

        protected:
        private:
            void getFullWindowSize(int clientWidth, int clientHeight,
                int* fullWidth, int* fullHeight);

            bool createContext(int inPixelFormat);
            int choosePixelFormat(const FrameBufferConfig* inConfig);
            FrameBufferConfig* getConfigs(unsigned int* outSize);
            const FrameBufferConfig* chooseConfig(
                const FrameBufferConfig* inConfig,
                const FrameBufferConfig* inConfigs, unsigned int inCount);
            int getPixelFormatAttrib(int inPixelFormat, int inAttrib);

            bool mDispatchResize;
            HWND mWND;
            HDC mDC;
            HGLRC mRC;
            HINSTANCE mINSTANCE;

            DWORD mStyle;
            DWORD mExStyle;

            uint16 mWidth;
            uint16 mHeight;

            static void setupExtensions();

            static bool mDidSetupGlew;
    };
}



#endif
