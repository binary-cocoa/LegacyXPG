#ifndef XPGH_MUTEX
#define XPGH_MUTEX

#include "Platforms.hpp"

#ifdef XPG_PLATFORM_UNIX
#   include <pthread.h>
#elif defined(XPG_PLATFORM_WINDOWS)
#   include "win32/windows.hpp"
#endif

namespace XPG
{
    class Mutex
    {
        public:
            Mutex();
            ~Mutex();

            void lock();
            void unlock();

        private:
#ifdef XPG_PLATFORM_WINDOWS
            HANDLE mMutex;
#elif defined(XPG_PLATFORM_UNIX)
            pthread_mutex_t mMutex;
#endif
    };
}

#endif

