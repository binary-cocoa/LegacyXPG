#ifndef XPGH_TIMER
#define XPGH_TIMER

#include "DataTypes.hpp"

namespace XPG
{
    void Idle(uint32 inMilliseconds = 1);
    void TimerStart();
    uint64 GetTicks();
}

#endif
