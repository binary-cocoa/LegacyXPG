#ifndef XPGH_DATATYPES
#define XPGH_DATATYPES

#include <stdint.h>

#include "Matrix3D.hpp"
#include "Vector3D.hpp"

typedef int_fast8_t   int8s;
typedef int_least16_t int16s;
typedef int_least32_t int32s;
typedef int_least64_t int64s;

typedef uint8_t  int8u;
typedef uint16_t int16u;
typedef uint32_t int32u;
typedef uint64_t int64u;

typedef XPG::Matrix4x4<float> mat4f;

typedef XPG::Vector2<float> vec2f;
typedef XPG::Vector3<float> vec3f;
typedef XPG::Vector4<float> vec4f;

typedef XPG::Vector2<double> vec2d;
typedef XPG::Vector3<double> vec3d;
typedef XPG::Vector4<double> vec4d;

#endif
