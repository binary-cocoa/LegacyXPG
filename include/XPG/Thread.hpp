#ifndef XPGH_THREAD
#define XPGH_THREAD

#include "DataTypes.hpp"

namespace XPG
{
    typedef volatile const bool& StopFlag;

    class Task
    {
        public:
            virtual void run(StopFlag) = 0;
    };

    class Thread
    {
        public:
            Thread();
            ~Thread();

            void start(Task* inTask = NULL);
            void launch();
            void wait();
            inline bool isRunning() { return mRunning; }
            inline void stop() { mStop = true; }

        private:
            Task* mTask;
            volatile bool mReady;
            volatile bool mRunning;
            volatile bool mStop;

            struct PrivateData;
            PrivateData* mData;
    };
}

#endif
