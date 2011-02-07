#include <XPG/Mutex.hpp>
#include <XPG/private/windows.hpp>
#include <cstdlib>

namespace XPG
{
    Mutex::Mutex()
    {
        mData = malloc(sizeof(HANDLE));
        *((HANDLE*)mData) = CreateMutex(NULL, FALSE, NULL);
    }

    Mutex::~Mutex()
    {
        CloseHandle(*((HANDLE*)mData));
        free(mData);
    }

    void Mutex::lock()
    {
        //DWORD waitResult =
        WaitForSingleObject(*((HANDLE*)mData), INFINITE);
        /// TODO: add error checking
    }

    void Mutex::unlock()
    {
        if (!ReleaseMutex(*((HANDLE*)mData)))
        {
            /// TODO: add error checking
        }
    }
}
