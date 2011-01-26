/**
 *  The OS-specific macro names are random/irregular and annoying to deal
 *  with. XPG unifies them all here and offers up a consistent naming
 *  convention.
 */

#ifndef XPGH_PLATFORMS
#define XPGH_PLATFORMS

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   define XPG_PLATFORM_STRING "Windows"
#   define XPG_PLATFORM_WINDOWS
#else
#   define XPG_PLATFORM_UNIX
#   define XPG_UNIX_HOME getenv("HOME")
#   if defined(__APPLE__) || defined(__APPLE_CC__)
#       define XPG_PLATFORM_STRING "OSX"
#       define XPG_PLATFORM_OSX
#   elif defined(__ANDROID__) || defined(ANDROID)
#       define XPG_PLATFORM_STRING "Android"
#       define XPG_PLATFORM_MOBILE
#       define XPG_PLATFORM_ANDROID
#   else
#       define XPG_PLATFORM_STRING "Linux"
#       define XPG_PLATFORM_LINUX
#   endif
#endif

#ifdef XPG_PLATFORM_ANDROID
#   include <stdlib.h>
#else
#   include <cstdlib>
#endif

#endif
