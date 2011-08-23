#ifndef XPGH_DATATYPES
#define XPGH_DATATYPES

#include <stdint.h>

#include "Matrix3D.hpp"
#include "Vector3D.hpp"

namespace XPG
{
    typedef int_fast8_t   int8;
    typedef int_least16_t int16;
    typedef int_least32_t int32;
    typedef int_least64_t int64;

    typedef uint8_t  uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    typedef Matrix4x4<float> mat4f;

    typedef Vector2<float> vec2f;
    typedef Vector3<float> vec3f;
    typedef Vector4<float> vec4f;

    typedef Vector2<double> vec2d;
    typedef Vector3<double> vec3d;
    typedef Vector4<double> vec4d;
}

#endif
