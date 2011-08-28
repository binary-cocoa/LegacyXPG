#ifndef XPGH_WINDOW
#define XPGH_WINDOW

#include <XPG/Engine.hpp>
#include <XPG/private/windows.hpp>
#include <XPG/private/glew.h>
#include <XPG/private/wglew.h>

namespace XPG
{
    class Window
    {
        public:
            Window(HINSTANCE inHINSTANCE, Engine::Settings& inSettings,
                bool inLegacy,
                Fullscreen::Mode inMode = Fullscreen::Off,
                const char* inTitle = NULL);
            virtual ~Window();

            bool getEvent(Event& inEvent);
            void softFullscreen(bool inFullscreen);
            void setTitle(const char* inTitle);

            inline void querySize(RECT& inRECT)
            {
                GetWindowRect(mHWND, &inRECT);
            }

            inline void show(int inCommand)
            {
                ShowWindow(mHWND, inCommand);
            }

            inline void update()
            {
                UpdateWindow(mHWND);
            }

            inline void swapBuffers()
            {
                SwapBuffers(mHDC);
            }

        protected:
            void setupLegacyDC();
            void setupNormalDC();

            LPTSTR mClassName;
            HINSTANCE mHINSTANCE;
            Engine::Settings& mSettings;
            HWND mHWND;
            HGLRC mHGLRC;
            HDC mHDC;
            int mFormerWidth;
            int mFormerHeight;
    };
}

#endif
