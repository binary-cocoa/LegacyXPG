#include <XPG/Timer.hpp>
#include <XPG/Platforms.hpp>

#include <sys/time.h>
#include <unistd.h>

#ifdef XPG_PLATFORM_ANDROID
#   include <time.h>
#endif

namespace XPG
{
    static uint64 mStart;

    void TimerStart()
    {
#ifdef XPG_PLATFORM_ANDROID
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        mStart = static_cast<uint64>(now.tv_sec * 1000)
            + static_cast<uint64>(now.tv_nsec / 1000000);
#else
        timeval now;
        gettimeofday(&now, NULL);
        mStart = static_cast<uint64>(now.tv_sec * 1000)
            + static_cast<uint64>(now.tv_usec / 1000);
#endif
    }

    void Idle(uint32 inMilliseconds)
    {
        usleep(inMilliseconds * 1000);
    }

    uint64 GetTicks()
    {
#ifdef XPG_PLATFORM_ANDROID
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        uint64 current = static_cast<uint64>(now.tv_sec * 1000)
            + static_cast<uint64>(now.tv_nsec / 1000000);
#else
        timeval now;
        gettimeofday(&now, NULL);
        uint64 current = static_cast<uint64>(now.tv_sec * 1000)
            + static_cast<uint64>(now.tv_usec / 1000);
#endif
        return current - mStart;
    }
}
