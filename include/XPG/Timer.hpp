#ifndef XPGH_TIMER
#define XPGH_TIMER

#include "Platforms.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    void Idle(int32u inMilliseconds = 1);
    void TimerStart();
    int64u GetTicks();
}

#endif
