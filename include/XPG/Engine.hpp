#ifndef XPGH_ENGINE
#define XPGH_ENGINE

#include "Event.hpp"
#include "Module.hpp"

namespace XPG
{
    class Engine
    {
    public:
        struct Version
        {
            Version();

            int16u vMajor;
            int16u vMinor;
        };

        struct Settings
        {
            Settings();

            int16u width;
            int16u height;
            int16u depth;
            bool fullScreen;
            bool legacyContext;
            Version context;
            Version shader;
        };

        Engine(const Settings& inSettings = Settings());
        ~Engine();

        bool getEvent(Event& inEvent);
        void swapBuffers();
        void setWindowTitle(const char* inTitle);
        void setIconTitle(const char* inTitle);

        void runModule(Module& inModule);

        const Settings& settings;

    private:
        void begin();
        void end();

        struct PrivateData;

        PrivateData* mData;
        bool mActive;
        Settings mSettings;
    };
}

#endif

