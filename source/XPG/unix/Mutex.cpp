#include <XPG/Mutex.hpp>

namespace XPG
{
    Mutex::Mutex()
    {
        pthread_mutex_init(&mMutex, NULL);
    }

    Mutex::~Mutex()
    {
        pthread_mutex_destroy(&mMutex);
    }

    void Mutex::lock()
    {
        pthread_mutex_lock(&mMutex);
    }

    void Mutex::unlock()
    {
        pthread_mutex_unlock(&mMutex);
    }
}
