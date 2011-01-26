#ifndef XPGH_MODULE
#define XPGH_MODULE

#include "OpenGL.hpp"
#include "DataTypes.hpp"
#include "Event.hpp"

namespace XPG
{
    class Module
    {
        public:
            Module();
            virtual ~Module();

            inline bool isRunning() { return mRunning; }
            inline void startRunning() { mRunning = true; }
            inline void stopRunning() { mRunning = false; }

            virtual void onDisplay() = 0;
            virtual void handleEvent(const Event& inEvent) = 0;

        private:
            bool mRunning;
    };
}

#endif

