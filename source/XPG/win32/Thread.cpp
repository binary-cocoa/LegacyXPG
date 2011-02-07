#include <XPG/Thread.hpp>
#include <XPG/private/windows.hpp>

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
        inThread->mStop = false;
        inThread->mRunning = true;
        inThread->run();
        inThread->mRunning = false;
    }

    struct Thread::PrivateData
    {
        Data data;
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
        mData->data.thread = this;
        mData->data.launcher = &startThread;
        mData->thread = CreateThread(NULL, 0, createThread, &mData->data, 0,
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
