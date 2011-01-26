#include "../Timer.hpp"
#include "windows.hpp"

namespace XPG
{
   static int64u mStart;

    void TimerStart()
    {
        mStart = GetTickCount();
    }

    void Idle(int32u inMilliseconds)
    {
        Sleep(inMilliseconds);
    }

    int64u GetTicks()
    {
        return GetTickCount() - mStart;
    }
}
