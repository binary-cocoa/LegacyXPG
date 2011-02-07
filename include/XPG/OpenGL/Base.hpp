#ifndef XPGH_OPENGL
#define XPGH_OPENGL

#include "../Platforms.hpp"

#ifdef XPG_PLATFORM_ANDROID
#   define XPG_OPENGL_ES
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
    typedef char GLchar; // small fix to keep tools consistent
#else
#   include "../private/glew.h"
#endif

#ifndef XPG_PLATFORM_ANDROID
#   include <iostream>
#   define LDB std::cout << __FILE__ << "\n\t" << __LINE__ << " -- " \
                         << XPG::OpenGLErrorString() << std::endl
#else
#   define LDB
#endif

namespace XPG
{
    const char* OpenGLErrorString();
}

#endif
