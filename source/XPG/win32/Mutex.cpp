#include <XPG/Mutex.hpp>

namespace XPG
{
    Mutex::Mutex()
    {
        mMutex = CreateMutex(NULL, FALSE, NULL);
    }

    Mutex::~Mutex()
    {
        CloseHandle(mMutex);
    }

    void Mutex::lock()
    {
        //DWORD waitResult =
        WaitForSingleObject(mMutex, INFINITE);
        /// TODO: add error checking
    }

    void Mutex::unlock()
    {
        if (!ReleaseMutex(mMutex))
        {
            /// TODO: add error checking
        }
    }
}
