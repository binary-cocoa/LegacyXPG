#include <XPG/Thread.hpp>
#include <cerrno>
#include <pthread.h>

namespace XPG
{
    void* createThread(void* inData)
    {
        Thread* t = static_cast<Thread*>(inData);
        t->launch();
        pthread_exit(NULL);
        return NULL;
    }

    struct Thread::PrivateData
    {
        pthread_t thread;
    };

    Thread::Thread() : mTask(NULL), mReady(false), mRunning(false), mStop(false)
    {
        mData = new PrivateData;
    }

    Thread::~Thread()
    {
        delete mData;
    }

    void Thread::start(Task* inTask)
    {
        if (mRunning) return;

        if (inTask)
            mTask = inTask;
        else if (!mTask)
            return;

        mReady = true;
        mRunning = true;
        switch (pthread_create(&mData->thread, NULL, createThread, this))
        {
            case 0: break; // no error

            case EAGAIN: // The system lacked resources
            case EINVAL: // invalid attributes
            case EPERM: // Caller does not have appropriate permission
            default: // something else went wrong
            {
                mRunning = false;
            }
        }
    }

    void Thread::launch()
    {
        // This function may only ever be called from createThread (which is
        // called from Thread::start).
        if (!mReady) return;
        mReady = false;
        mStop = false;
        mTask->run(mStop);
        mRunning = false;
    }

    void Thread::wait()
    {
        switch (pthread_join(mData->thread, NULL))
        {
            case 0: break; // no error

            case EINVAL: break; // mData->thread does not refer to a joinable thread
            case ESRCH: break; // No thread found corresponding to mData->thread
            case EDEADLK: break; // A deadlock was detected.
            default: {} // something else went wrong
        }
    }
}
