#ifndef XPGH_THREAD
#define XPGH_THREAD

#include "DataTypes.hpp"

namespace XPG
{
    class Thread
    {
        public:
            struct Data
            {
                Thread* thread;
                void (*launcher)(Thread*);
            };

            Thread();
            ~Thread();

            void start();
            void wait();

            virtual void run();

            inline bool isRunning() { return mRunning; }
            inline void stop() { mStop = true; }

        private:
            static void startThread(Thread* inThread);

            volatile bool mRunning;
            volatile bool mStop;

            struct PrivateData;
            PrivateData* mData;
    };
}

#endif
