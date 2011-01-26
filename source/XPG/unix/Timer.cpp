#include <XPG/Timer.hpp>

#include <sys/time.h>
#include <unistd.h>

#ifdef XPG_PLATFORM_ANDROID
#   include <time.h>
#endif

namespace XPG
{
    static int64u mStart;

    void TimerStart()
    {
#ifdef XPG_PLATFORM_ANDROID
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        mStart = static_cast<int64u>(now.tv_sec * 1000)
            + static_cast<int64u>(now.tv_nsec / 1000000);
#else
        timeval now;
        gettimeofday(&now, NULL);
        mStart = static_cast<int64u>(now.tv_sec * 1000)
            + static_cast<int64u>(now.tv_usec / 1000);
#endif
    }

    void Idle(int32u inMilliseconds)
    {
        usleep(inMilliseconds * 1000);
    }

    int64u GetTicks()
    {
#ifdef XPG_PLATFORM_ANDROID
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        int64u current = static_cast<int64u>(now.tv_sec * 1000)
            + static_cast<int64u>(now.tv_nsec / 1000000);
#else
        timeval now;
        gettimeofday(&now, NULL);
        int64u current = static_cast<int64u>(now.tv_sec * 1000)
            + static_cast<int64u>(now.tv_usec / 1000);
#endif
        return current - mStart;
    }
}
