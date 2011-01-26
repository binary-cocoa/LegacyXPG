#include <XPG/Display.hpp>

namespace XPG
{
    Context::Version::Version() : vMajor(0), vMinor(0)
    {

    }

    Context::Parameters::Parameters() : width(640), height(480), depth(0),
        fullScreen(false), legacyContext(false)
    {

    }
}
