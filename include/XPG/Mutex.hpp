#ifndef XPGH_MUTEX
#define XPGH_MUTEX

namespace XPG
{
    class Mutex
    {
    public:
        Mutex();
        ~Mutex();

        void lock();
        void unlock();

    private:
        void* mData;
    };
}

#endif

