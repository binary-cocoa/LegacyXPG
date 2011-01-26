#include "../Thread.hpp"

#include "windows.hpp"

namespace XPG
{
    DWORD WINAPI createThread(LPVOID inData)
    {
        Thread* t = static_cast<Thread*>(inData);
        t->launch();
        return 0;
    }

    struct Thread::PrivateData
    {
        DWORD ID;
        HANDLE thread;
    };

    Thread::Thread() : mTask(NULL), mReady(false), mRunning(false), mStop(false)
    {
        mData = new PrivateData;
    }

    Thread::~Thread()
    {
        /// TODO: add more careful error handling
        CloseHandle(mData->thread);
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
        mData->thread = CreateThread(NULL, 0, createThread, this, 0,
            &mData->ID);
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
        WaitForSingleObject(mData->thread, INFINITE);
    }
}
