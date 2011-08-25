#ifndef XPGH_ENGINE
#define XPGH_ENGINE

#include "Event.hpp"
#include "Module.hpp"

namespace XPG
{
    namespace Fullscreen
    {
        enum Mode { Off, Soft, Hard };
    }

    namespace Context
    {
        enum Profile { Legacy, GL32, Custom };
    }

    class Engine
    {
    public:
        struct Version
        {
            uint16 vMajor;
            uint16 vMinor;

            Version() : vMajor(0), vMinor(0) {}
        };

        struct Settings
        {
            uint16 width;
            uint16 height;
            uint16 depth;
            Fullscreen::Mode fullscreen;
            Context::Profile profile;
            Version context;
            Version shader;

            Settings() : width(640), height(360), depth(0),
                fullscreen(Fullscreen::Off), profile(Context::GL32) {}
        };

        Engine(const Settings& inSettings = Settings());
        ~Engine();

        bool getEvent(Event& inEvent);
        void swapBuffers();
        void setWindowTitle(const char* inTitle);
        void setIconTitle(const char* inTitle);
        void setFullscreen(Fullscreen::Mode inMode);

        void runModule(Module& inModule);

        inline const Settings& settings() const { return mSettings; }

    private:
        void* mMeta;
        Settings mSettings;
    };
}

#endif

