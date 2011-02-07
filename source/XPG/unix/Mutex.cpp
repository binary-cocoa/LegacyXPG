#include <XPG/Mutex.hpp>
#include <pthread.h>

#include <XPG/Platforms.hpp>
#ifdef XPG_PLATFORM_ANDROID
#   include <stdlib.h>
#else
#   include <cstdlib>
#endif

namespace XPG
{
    Mutex::Mutex()
    {
        mData = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init((pthread_mutex_t*)mData, NULL);
    }

    Mutex::~Mutex()
    {
        pthread_mutex_destroy((pthread_mutex_t*)mData);
        free(mData);
    }

    void Mutex::lock()
    {
        pthread_mutex_lock((pthread_mutex_t*)mData);
    }

    void Mutex::unlock()
    {
        pthread_mutex_unlock((pthread_mutex_t*)mData);
    }
}
