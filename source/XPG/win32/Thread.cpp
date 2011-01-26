#include <XPG/Thread.hpp>
#include <XPG/windows.hpp>

namespace XPG
{
    DWORD WINAPI createThread(LPVOID inData)
    {
        Thread::Data* data = static_cast<Thread::Data*>(inData);
        (*data->launcher)(data->thread);
        return 0;
    }

    void Thread::startThread(Thread* inThread)
    {
        inThread->mRunning = true;
        inThread->run();
        inThread->mRunning = false;
    }

    struct Thread::PrivateData
    {
        DWORD ID;
        HANDLE thread;
    };

    Thread::Thread() : mRunning(false), mStop(false)
    {
        mData = new PrivateData;
    }

    Thread::~Thread()
    {
        /// TODO: add more careful error handling
        CloseHandle(mData->thread);
        delete mData;
    }

    void Thread::start()
    {
        if (mRunning) return;

        mRunning = true;
        Data d;
        d.thread = this;
        d.launcher = &startThread;
        mData->thread = CreateThread(NULL, 0, createThread, &d, 0,
            &mData->ID);
    }

    void Thread::wait()
    {
        WaitForSingleObject(mData->thread, INFINITE);
    }

    void Thread::run()
    {
    }
}
