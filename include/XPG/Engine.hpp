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

            uint16 vMajor;
            uint16 vMinor;
        };

        struct Settings
        {
            Settings();

            uint16 width;
            uint16 height;
            uint16 depth;
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

