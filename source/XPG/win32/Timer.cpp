#include <XPG/Timer.hpp>
#include <XPG/private/windows.hpp>

namespace XPG
{
   static uint64 mStart;

    void TimerStart()
    {
        mStart = GetTickCount();
    }

    void Idle(uint32 inMilliseconds)
    {
        Sleep(inMilliseconds);
    }

    uint64 GetTicks()
    {
        return GetTickCount() - mStart;
    }
}
