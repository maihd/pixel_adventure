#pragma once

#include <stdint.h>

// -------------------------------------------------------------
// Data structure alignment
// -------------------------------------------------------------

#ifndef ALIGNAS
#   if defined(__cplusplus)
#       define ALIGNAS(TYPE_NAME, ALIGNMENT) alignas(ALIGNMENT) TYPE_NAME
#   elif defined(__GNUC__)
#       define ALIGNAS(TYPE_NAME, ALIGNMENT) TYPE_NAME __attribute__((aligned(ALIGNMENT)))
#   elif defined(_MSC_VER)
#       define ALIGNAS(TYPE_NAME, ALIGNMENT) __declspec(align(ALIGNMENT)) TYPE_NAME
#   else
#       error The compiler is not support alignas operator.
#   endif
#endif

// -------------------------------------------------------------
// SIMD Supporting
// -------------------------------------------------------------

#ifndef VECTORMATH_SIMD_ENABLE
#define VECTORMATH_SIMD_ENABLE 1
#endif

// Detect neon support & enable
#if defined(__ARM_NEON) && defined(VECTORMATH_SIMD_ENABLE) && VECTORMATH_SIMD_ENABLE
#   if defined(__aarch64__) && defined(__ANDROID__)
#       define VECTORMATH_NEON_SUPPORT 0
#   else
#       define VECTORMATH_NEON_SUPPORT 1
#   endif         
#else
#   define VECTORMATH_NEON_SUPPORT 0
#endif

// Detect SSE support & enable
#define VECTORMATH_SSE_SUPPORT 0

#if defined(__SSSE3__)
#   define VECTORMATH_SSE_SUPPORT 1
#endif

#if defined(__SSE__) || defined(__SSE2__) || defined(__SSE3__)
#   undef  VECTORMATH_SSE_SUPPORT
#   define VECTORMATH_SSE_SUPPORT 1
#endif

#if defined(__SSE4_1__) || defined(__SSE4_2__) || defined(__SSE_MATH__)
#   undef  VECTORMATH_SSE_SUPPORT
#   define VECTORMATH_SSE_SUPPORT 1
#endif

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_IX64))
#   if defined(_M_HYBRID_X86_ARM64)
#       undef  VECTORMATH_SSE_SUPPORT
#       define VECTORMATH_SSE_SUPPORT 0
#   else
#       undef  VECTORMATH_SSE_SUPPORT
#       define VECTORMATH_SSE_SUPPORT 1
#   endif
#endif

#if (defined(__AVX__) || defined(__AVX2__) || defined(_M_AMD64) || defined(_M_X64) || (_M_IX86_FP == 1) || (_M_IX86_FP == 2))
#   undef  VECTORMATH_SSE_SUPPORT
#   define VECTORMATH_SSE_SUPPORT 1
#endif

// Disable SIMD on unsupported CPU
#if !VECTORMATH_SSE_SUPPORT && !VECTORMATH_NEON_SUPPORT
#   undef  VECTORMATH_SIMD_ENABLE
#   define VECTORMATH_SIMD_ENABLE 0
#endif

// Define __m128
#if VECTORMATH_SSE_SUPPORT
#include <emmintrin.h>
#elif VECTORMATH_NEON_SUPPORT
#include <arm_neon.h>
typedef float32x2_t __m64;
typedef float32x4_t __m128;
typedef int32x4_t   __m128i;
#else
typedef struct __m64   { float      data[2]; } __m64;
typedef struct __m128  { float      data[4]; } __m128;
typedef struct __m128i { int32_t    data[4]; } __m128i;
#endif

// -------------------------------------------------------------
// Types
// -------------------------------------------------------------

/// vec2
/// 2D floating-point vector
typedef struct vec2
{
    float                       x;
    float                       y;
} vec2;

/// vec3
/// 3D floating-point vector
typedef union ALIGNAS(vec3, 16)
{
    struct
    {
        float                   x;
        float                   y;
        float                   z;
    };
    vec2                        xy;
    __m128                      m128;
} vec3;

/// vec4
/// 4D floating-point vector
typedef union ALIGNAS(vec4, 16)
{
    struct
    {
        float                   x;
        float                   y;
        float                   z;

        union
        {

            float               w;
            float               angle;
        };
    };

    struct
    {
        vec2                    xy;
        vec2                    zw;
    };

    vec3                        xyz;
    vec3                        axis;

    __m128                      m128;
} vec4;

/// ivec2
/// 2D integer vector
typedef struct ivec2
{
    int32_t                     x, y;
} ivec2;

/// ivec3
/// 3D integer vector
typedef union ALIGNAS(ivec3, 16)
{
    struct
    {
        int32_t                 x, y, z;
    };

    __m128i                     m128i;
} ivec3;

/// ivec4
/// 4D integer vector
typedef union ALIGNAS(ivec4, 16)
{
    struct
    {
        int32_t                 x, y, z, w;
    };

    __m128i                     m128i;
} ivec4;

/// ivec2
/// 2D integer vector
typedef struct uvec2
{
    uint32_t                    x, y;
} uvec2;

/// ivec3
/// 3D integer vector
typedef union ALIGNAS(uvec3, 16)
{
    struct
    {
        uint32_t                x, y, z;
    };

    __m128i                     m128i;
} uvec3;

/// ivec4
/// 4D integer vector
typedef union ALIGNAS(uvec4, 16)
{
    struct
    {
        uint32_t                x, y, z, w;
    };

    __m128i                     m128i;
} uvec4;

/// mat2
/// 2x2 floating-point matrix
typedef union ALIGNAS(mat2, 16)
{
    struct
    {
        vec2                    row0;
        vec2                    row1;
    };

    struct
    {
        float                   m00, m01;
        float                   m10, m11;
    };
    __m128                      m128;
} mat2;

/// mat3
/// 3x3 floating-point matrix
typedef union ALIGNAS(mat3, 16)
{
    struct
    {
        vec3                    row0;
        vec3                    row1;
        vec3                    row2;
    };

    struct
    {
        float                   m00, m01, m02, _m03;
        float                   m10, m11, m12, _m13;
        float                   m20, m21, m22, _m23;
    };
} mat3;

/// mat4
/// 4x4 floating-point matrix
typedef union ALIGNAS(mat4, 16)
{
    struct
    {
        vec4                    row0;
        vec4                    row1;
        vec4                    row2;
        vec4                    row3;
    };

    struct
    {
        float                   m00, m01, m02, m03;
        float                   m10, m11, m12, m13;
        float                   m20, m21, m22, m23;
        float                   m30, m31, m32, m33;
    };
} mat4;
