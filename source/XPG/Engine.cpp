#include <XPG/Engine.hpp>

namespace XPG
{
    Engine::Version::Version() : vMajor(0), vMinor(0)
    {
    }

    Engine::Settings::Settings() : width(640), height(480), depth(0),
        fullScreen(false), legacyContext(false)
    {
    }

    Engine::Engine(const Settings& inSettings) : settings(mSettings),
        mActive(false)
    {
        mSettings = inSettings;
        begin();
    }

    Engine::~Engine()
    {
        end();
    }
}
