#include <XPG/Thread.hpp>
#include <cerrno>
#include <pthread.h>

namespace XPG
{
    void* createThread(void* inData)
    {
        Thread::Data* data = static_cast<Thread::Data*>(inData);
        (*data->launcher)(data->thread);
        pthread_exit(NULL);
        return NULL;
    }

    void Thread::startThread(Thread* inThread)
    {
        inThread->mStop = false;
        inThread->mRunning = true;
        inThread->run();
        inThread->mRunning = false;
    }

    struct Thread::PrivateData
    {
        Data data;
        pthread_t thread;
    };

    Thread::Thread() : mRunning(false), mStop(false)
    {
        mData = new PrivateData;
        mData->data.thread = this;
        mData->data.launcher = &startThread;
    }

    Thread::~Thread()
    {
        delete mData;
    }

    void Thread::start()
    {
        if (mRunning) return;

        mRunning = true;
        switch (pthread_create(&mData->thread, NULL, createThread,
            &mData->data))
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

    void Thread::run()
    {
    }
}
