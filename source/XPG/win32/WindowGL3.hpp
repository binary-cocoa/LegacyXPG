#ifndef WINDOWGL3_H
#define WINDOWGL3_H

#include "Window.hpp"

namespace XPG
{
    class WindowGL3 : public Window
    {
        public:
            WindowGL3(HINSTANCE inHINSTANCE, Engine::Settings& inSettings,
                Fullscreen::Mode inMode = Fullscreen::Off,
                const char* inTitle = NULL);
            virtual ~WindowGL3();

        protected:
            virtual void setupDC();
    };
}

#endif
