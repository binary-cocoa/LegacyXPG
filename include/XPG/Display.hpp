#ifndef XPGH_DISPLAY
#define XPGH_DISPLAY

#include "Event.hpp"
#include "Module.hpp"

namespace XPG
{
    class Context
    {
        public:
            Context();
            ~Context();

            struct Version
            {
                Version();

                int16u vMajor;
                int16u vMinor;
            };

            struct Parameters
            {
                Parameters();

                int32u width;
                int32u height;
                int32u depth;
                bool fullScreen;
                bool legacyContext;
                Version context;
                Version shader;
            };

            const Parameters& details;

            void create(const Parameters& inDetails = Parameters());
            void destroy();
            void swapBuffers();
            bool getEvent(Event& inEvent);
            void runModule(Module& inModule);
            void setWindowTitle(const char* inTitle);
            void setIconTitle(const char* inTitle);

        private:
            Parameters mDetails;

            struct PrivateData;
            PrivateData* mData;
    };
}

#endif
