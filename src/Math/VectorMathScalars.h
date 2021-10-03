#pragma once

#include <math.h>
#include "VectorTypes.h"

#undef min // When Windows.h was included, min is an macro
#undef max // When Windows.h was included, max is an macro

constexpr float PI = 3.141592653589f;

// Should not use both scalar & m128 versions of VectorMath together
#if (defined(__has_include) && __has_include("VectorMath.h"))
#error VectorMath.h has been include, please remove this from your source if you attempt to use scalar version of VectorMath.h
#endif

// -------------------------------------------------------------
// Operators, only support on C++
// -------------------------------------------------------------
#ifdef __cplusplus

__forceinline vec2 operator-(vec2 v)
{
    return vec2(-v.x, -v.y);
}

__forceinline vec2 operator+(vec2 v)
{
    return v;
}

__forceinline vec2& operator--(vec2& v)
{
    --v.x;
    --v.y;
    return v;
}

__forceinline vec2& operator++(vec2& v)
{
    ++v.x;
    ++v.y;
    return v;
}

__forceinline vec2 operator--(vec2& v, int)
{
    const vec2 result = v;

    v.x--;
    v.y--;

    return result;
}

__forceinline vec2 operator++(vec2& v, int)
{
    const vec2 result = v;

    v.x++;
    v.y++;

    return result;
}

__forceinline vec2 operator+(vec2 a, vec2 b)
{
    return vec2(a.x + b.x, a.y + b.y);
}

__forceinline vec2 operator-(vec2 a, vec2 b)
{
    return vec2(a.x - b.x, a.y - b.y);
}

__forceinline vec2 operator*(vec2 a, vec2 b)
{
    return vec2(a.x * b.x, a.y * b.y);
}

__forceinline vec2 operator/(vec2 a, vec2 b)
{
    return vec2(a.x / b.x, a.y / b.y);
}

__forceinline vec2 operator+(vec2 a, float b)
{
    return a + vec2(b);
}

__forceinline vec2 operator-(vec2 a, float b)
{
    return a - vec2(b);
}

__forceinline vec2 operator*(vec2 a, float b)
{
    return a * vec2(b);
}

__forceinline vec2 operator/(vec2 a, float b)
{
    return a / vec2(b);
}

__forceinline vec2 operator+(float a, vec2 b)
{
    return vec2(a) + b;
}

__forceinline vec2 operator-(float a, vec2 b)
{
    return vec2(a) - b;
}

__forceinline vec2 operator*(float a, vec2 b)
{
    return vec2(a) * b;
}

__forceinline vec2 operator/(float a, vec2 b)
{
    return vec2(a) / b;
}

__forceinline vec2& operator+=(vec2& a, vec2 b)
{
    return (a = a + b);
}

__forceinline vec2& operator+=(vec2& a, float b)
{
    return (a = a + b);
}

__forceinline vec2& operator-=(vec2& a, vec2 b)
{
    return (a = a - b);
}

__forceinline vec2& operator-=(vec2& a, float b)
{
    return (a = a - b);
}

__forceinline vec2& operator*=(vec2& a, vec2 b)
{
    return (a = a * b);
}

__forceinline vec2& operator*=(vec2& a, float b)
{
    return (a = a * b);
}

__forceinline vec2& operator/=(vec2& a, vec2 b)
{
    return (a = a / b);
}

__forceinline vec2& operator/=(vec2& a, float b)
{
    return (a = a + b);
}

__forceinline bool operator==(vec2 a, vec2 b)
{
    return a.x == b.x && a.y == b.y;
}

__forceinline bool operator!=(vec2 a, vec2 b)
{
    return a.x != b.x || a.y != b.y;
}

__forceinline vec3 operator-(vec3 v)
{
    return vec3(-v.x, -v.y, -v.z);
}

__forceinline vec3 operator+(vec3 v)
{
    return v;
}

__forceinline vec3& operator--(vec3& v)
{
    --v.x;
    --v.y;
    --v.z;
    return v;
}

__forceinline vec3& operator++(vec3& v)
{
    ++v.x;
    ++v.y;
    ++v.z;
    return v;
}

__forceinline vec3 operator--(vec3& v, int)
{
    const vec3 result = v;

    v.x--;
    v.y--;
    v.z--;

    return result;
}

__forceinline vec3 operator++(vec3& v, int)
{
    const vec3 result = v;

    v.x++;
    v.y++;
    v.z++;

    return result;
}

__forceinline vec3 operator+(vec3 a, vec3 b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

__forceinline vec3 operator-(vec3 a, vec3 b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

__forceinline vec3 operator*(vec3 a, vec3 b)
{
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

__forceinline vec3 operator/(vec3 a, vec3 b)
{
    return vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}

__forceinline vec3 operator+(vec3 a, float b)
{
    return vec3(a.x + b, a.y + b, a.z + b);
}

__forceinline vec3 operator-(vec3 a, float b)
{
    return vec3(a.x - b, a.y - b, a.z - b);
}

__forceinline vec3 operator*(vec3 a, float b)
{
    return vec3(a.x * b, a.y * b, a.z * b);
}

__forceinline vec3 operator/(vec3 a, float b)
{
    return vec3(a.x / b, a.y / b, a.z / b);
}

__forceinline vec3 operator+(float a, vec3 b)
{
    return vec3(a + b.x, a + b.y, a + b.z);
}

__forceinline vec3 operator-(float a, vec3 b)
{
    return vec3(a - b.x, a - b.y, a - b.z);
}

__forceinline vec3 operator*(float a, vec3 b)
{
    return vec3(a * b.x, a * b.y, a * b.z);
}

__forceinline vec3 operator/(float a, vec3 b)
{
    return vec3(a / b.x, a / b.y, a / b.z);
}

__forceinline vec3& operator+=(vec3& a, vec3 b)
{
    return (a = a + b);
}

__forceinline vec3& operator+=(vec3& a, float b)
{
    return (a = a + b);
}

__forceinline vec3& operator-=(vec3& a, vec3 b)
{
    return (a = a - b);
}

__forceinline vec3& operator-=(vec3& a, float b)
{
    return (a = a - b);
}

__forceinline vec3& operator*=(vec3& a, vec3 b)
{
    return (a = a * b);
}

__forceinline vec3& operator*=(vec3& a, float b)
{
    return (a = a * b);
}

__forceinline vec3& operator/=(vec3& a, vec3 b)
{
    return (a = a / b);
}

__forceinline vec3& operator/=(vec3& a, float b)
{
    return (a = a + b);
}

__forceinline bool operator==(vec3 a, vec3 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

__forceinline bool operator!=(vec3 a, vec3 b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

__forceinline vec4 operator-(vec4 v)
{
    return vec4(-v.x, -v.y, -v.z, -v.w);
}

__forceinline vec4 operator+(vec4 v)
{
    return v;
}

__forceinline vec4& operator--(vec4& v)
{
    --v.x;
    --v.y;
    --v.z;
    --v.w;
    return v;
}

__forceinline vec4& operator++(vec4& v)
{
    ++v.x;
    ++v.y;
    ++v.z;
    ++v.w;
    return v;
}

__forceinline vec4 operator--(vec4& v, int)
{
    const vec4 result = v;

    v.x--;
    v.y--;
    v.z--;
    v.w--;

    return result;
}

__forceinline vec4 operator++(vec4& v, int)
{
    const vec4 result = v;

    v.x++;
    v.y++;
    v.z++;
    v.w++;

    return result;
}

__forceinline vec4 operator+(vec4 a, vec4 b)
{
    return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

__forceinline vec4 operator-(vec4 a, vec4 b)
{
    return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

__forceinline vec4 operator*(vec4 a, vec4 b)
{
    return vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

__forceinline vec4 operator/(vec4 a, vec4 b)
{
    return vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

__forceinline vec4 operator+(vec4 a, float b)
{
    return vec4(a.x + b, a.y + b, a.z + b, a.w + b);
}

__forceinline vec4 operator-(vec4 a, float b)
{
    return vec4(a.x - b, a.y - b, a.z - b, a.w - b);
}

__forceinline vec4 operator*(vec4 a, float b)
{
    return vec4(a.x * b, a.y * b, a.z * b, a.w * b);
}

__forceinline vec4 operator/(vec4 a, float b)
{
    return vec4(a.x / b, a.y / b, a.z / b, a.w / b);
}

__forceinline vec4 operator+(float a, vec4 b)
{
    return vec4(a + b.x, a + b.y, a + b.z, a + b.w);
}

__forceinline vec4 operator-(float a, vec4 b)
{
    return vec4(a - b.x, a - b.y, a - b.z, a - b.w);
}

__forceinline vec4 operator*(float a, vec4 b)
{
    return vec4(a * b.x, a * b.y, a * b.z, a * b.w);
}

__forceinline vec4 operator/(float a, vec4 b)
{
    return vec4(a / b.x, a / b.y, a / b.z, a / b.w);
}

__forceinline vec4& operator+=(vec4& a, vec4 b)
{
    return (a = a + b);
}

__forceinline vec4& operator+=(vec4& a, float b)
{
    return (a = a + b);
}

__forceinline vec4& operator-=(vec4& a, vec4 b)
{
    return (a = a - b);
}

__forceinline vec4& operator-=(vec4& a, float b)
{
    return (a = a - b);
}

__forceinline vec4& operator*=(vec4& a, vec4 b)
{
    return (a = a * b);
}

__forceinline vec4& operator*=(vec4& a, float b)
{
    return (a = a * b);
}

__forceinline vec4& operator/=(vec4& a, vec4 b)
{
    return (a = a / b);
}

__forceinline vec4& operator/=(vec4& a, float b)
{
    return (a = a + b);
}

__forceinline bool operator==(vec4 a, vec4 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

__forceinline bool operator!=(vec4 a, vec4 b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

__forceinline mat4 operator-(mat4 m)
{
    mat4 result;
    result.row0 = -m.row0;
    result.row1 = -m.row1;
    result.row2 = -m.row2;
    result.row3 = -m.row3;
    return result;
}

__forceinline mat4 operator+(mat4 m)
{
    return m;
}

__forceinline mat4& operator--(mat4& m)
{
    --m.row0;
    --m.row1;
    --m.row2;
    --m.row3;
    return m;
}

__forceinline mat4& operator++(mat4& m)
{
    ++m.row0;
    ++m.row1;
    ++m.row2;
    ++m.row3;
    return m;
}

__forceinline mat4 operator--(mat4& m, int)
{
    m.row0--;
    m.row1--;
    m.row2--;
    m.row3--;
    return m;
}

__forceinline mat4 operator++(mat4& m, int)
{
    m.row0++;
    m.row1++;
    m.row2++;
    m.row3++;
    return m;
}

__forceinline mat4 operator+(mat4 a, mat4 b)
{
    return mat4(
        a.row0 + b.row0,
        a.row1 + b.row1,
        a.row2 + b.row2,
        a.row3 + b.row3
    );
}

__forceinline mat4 operator+(mat4 a, float b)
{
    return mat4(
        a.row0 + b,
        a.row1 + b,
        a.row2 + b,
        a.row3 + b
    );
}

__forceinline mat4 operator+(float a, mat4 b)
{
    return mat4(
        a + b.row0,
        a + b.row1,
        a + b.row2,
        a + b.row3
    );
}

__forceinline mat4 operator-(mat4 a, mat4 b)
{
    return mat4(
        a.row0 - b.row0,
        a.row1 - b.row1,
        a.row2 - b.row2,
        a.row3 - b.row3
    );
}

__forceinline mat4 operator-(mat4 a, float b)
{
    return mat4(
        a.row0 - b,
        a.row1 - b,
        a.row2 - b,
        a.row3 - b
    );
}

__forceinline mat4 operator-(float a, mat4 b)
{
    return mat4(
        a - b.row0,
        a - b.row1,
        a - b.row2,
        a - b.row3
    );
}

__forceinline mat4 operator*(mat4 a, mat4 b)
{
    return mat4(
        a.row0 * b.row0,
        a.row1 * b.row1,
        a.row2 * b.row2,
        a.row3 * b.row3
    );
}

__forceinline mat4 operator*(mat4 a, float b)
{
    return mat4(
        a.row0 * b,
        a.row1 * b,
        a.row2 * b,
        a.row3 * b
    );
}

__forceinline mat4 operator*(float a, mat4 b)
{
    return mat4(
        a * b.row0,
        a * b.row1,
        a * b.row2,
        a * b.row3
    );
}

__forceinline mat4 operator/(mat4 a, mat4 b)
{
    return mat4(
        a.row0 / b.row0,
        a.row1 / b.row1,
        a.row2 / b.row2,
        a.row3 / b.row3
    );
}

__forceinline mat4 operator/(mat4 a, float b)
{
    return mat4(
        a.row0 / b,
        a.row1 / b,
        a.row2 / b,
        a.row3 / b
    );
}

__forceinline mat4 operator/(float a, mat4 b)
{
    return mat4(
        a / b.row0,
        a / b.row1,
        a / b.row2,
        a / b.row3
    );
}

__forceinline mat4& operator+=(mat4& a, mat4 b)
{
    return (a = a + b);
}

__forceinline mat4& operator+=(mat4& a, float b)
{
    return (a = a + b);
}

__forceinline mat4& operator-=(mat4& a, mat4 b)
{
    return (a = a - b);
}

__forceinline mat4& operator-=(mat4& a, float b)
{
    return (a = a - b);
}

__forceinline mat4& operator*=(mat4& a, mat4 b)
{
    return (a = a * b);
}

__forceinline mat4& operator*=(mat4& a, float b)
{
    return (a = a * b);
}

__forceinline mat4& operator/=(mat4& a, mat4 b)
{
    return (a = a / b);
}

__forceinline mat4& operator/=(mat4& a, float b)
{
    return (a = a + b);
}

__forceinline bool operator==(mat4 a, mat4 b)
{
    return a.row0 == b.row0 && a.row1 == b.row1 && a.row2 == b.row2 && a.row3 == b.row3;
}

__forceinline bool operator!=(mat4 a, mat4 b)
{
    return a.row0 == b.row0 || a.row1 == b.row1 || a.row2 == b.row2 || a.row3 == b.row3;
}

#endif //! OPERATORS

// -------------------------------------------------------------
// Functions
// -------------------------------------------------------------

/// Convert degrees to radians
__forceinline float radians(float degrees)
{
    return degrees * PI / 180.0f;
}

/// Convert radians to degrees
__forceinline float degrees(float radians)
{
    return radians * 180.0f / PI;
}

/// Compute the sign of 'x'
__forceinline int sign(int x)
{
    return x >> 31;
}

/// Get the smaller value
__forceinline int min(int x, int y)
{
    return x < y ? x : y;
}

/// Get the larger value
__forceinline int max(int x, int y)
{
    return x > y ? x : y;
}

/// Clamps the 'x' to the [min, max]
__forceinline int clamp(int x, int min, int max)
{
    return x < min ? min : (x > max ? max : x);
}

/// Computes sign of 'x'
__forceinline int signf(float x)
{
    union
    {
        float f;
        int   i;
    } cvt;
    cvt.f = x;
    return sign(cvt.i);
}

/// Get the fractal part of floating point
__forceinline float fracf(float x)
{
    float y;
    return modff(x, &y);
}

// Android polyfill for log2 and log2f
#if defined(__ANDROID__)
__forceinline float log2f(float x)
{
    return (logf(x) / 0.693147180559945f);
}

__forceinline double log2(double x)
{
    return (log(x) / 0.693147180559945);
}
#endif

/// Get the smaller value
__forceinline float minf(float x, float y)
{
    return x < y ? x : y;
}

/// Get the larger value
__forceinline float maxf(float x, float y)
{
    return x > y ? x : y;
}

/// Clamps the 'x' value to the [min, max].
__forceinline float clampf(float x, float min, float max)
{
    return x < min ? min : (x > max ? max : x);
}

/// Clamps the specified value within the range of 0 to 1
__forceinline float saturatef(float x)
{
    return clampf(x, 0.0f, 1.0f);
}

/// Compares two values, returning 0 or 1 based on which value is greater.
__forceinline float stepf(float y, float x)
{
    return x >= y;
}

/// Performs a linear interpolation.
__forceinline float lerpf(float x, float y, float s)
{
    return x + (y - x) * s;
}

/// Computes inverse square root of 'x'.
__forceinline float rsqrtf(float x)
{
    return 1.0f / sqrtf(x);
}

/// Compute a smooth Hermite interpolation
/// @return: 0 if x <= min
///          1 if x >= max
///          (0, 1) otherwise
__forceinline float smoothstepf(float min, float max, float x)
{
    return (clampf(x, min, max) - min) / (max - min);
}

/// Computes sign of 'x'
__forceinline ivec3 vec3_sign(vec3 v)
{
    return ivec3 { signf(v.x), signf(v.y), signf(v.z) };
}

/// Computes absolute value
__forceinline vec3 vec3_abs(vec3 v)
{
    return vec3(fabsf(v.x), fabsf(v.y), fabsf(v.z));
}

/// Computes cosine
__forceinline vec3 vec3_cos(vec3 v)
{
    return vec3(cosf(v.x), cosf(v.y), cosf(v.z));
}

/// Computes sine
__forceinline vec3 vec3_sin(vec3 v)
{
    return vec3(sinf(v.x), sinf(v.y), sinf(v.z));
}

/// Computes tangent
__forceinline vec3 vec3_tan(vec3 v)
{
    return vec3(tanf(v.x), tanf(v.y), tanf(v.z));
}

/// Computes hyperbolic cosine
__forceinline vec3 vec3_cosh(vec3 v)
{
    return vec3(coshf(v.x), coshf(v.y), coshf(v.z));
}

/// Computes hyperbolic sine
__forceinline vec3 vec3_sinh(vec3 v)
{
    return vec3(sinhf(v.x), sinhf(v.y), sinhf(v.z));
}

/// Computes hyperbolic tangent
__forceinline vec3 vec3_tanh(vec3 v)
{
    return vec3(tanhf(v.x), tanhf(v.y), tanhf(v.z));
}

/// Computes inverse cosine
__forceinline vec3 vec3_acos(vec3 v)
{
    return vec3(acosf(v.x), acosf(v.y), acosf(v.z));
}

/// Computes inverse sine
__forceinline vec3 vec3_asin(vec3 v)
{
    return vec3(asinf(v.x), asinf(v.y), asinf(v.z));
}

/// Computes inverse tangent
__forceinline vec3 vec3_atan(vec3 v)
{
    return vec3(atanf(v.x), atanf(v.y), asinf(v.z));
}

/// Computes inverse tangent with 2 args
__forceinline vec3 vec3_atan2(vec3 a, vec3 b)
{
    return vec3(atan2f(a.x, b.x), atan2f(a.y, b.y), atan2f(a.z, b.z));
}

/// Computes Euler number raised to the power 'x'
__forceinline vec3 vec3_exp(vec3 v)
{
    return vec3(expf(v.x), expf(v.y), expf(v.z));
}

/// Computes 2 raised to the power 'x'
__forceinline vec3 exp2(vec3 v)
{
    return vec3(exp2f(v.x), exp2f(v.y), exp2f(v.z));
}

/// Computes the base Euler number logarithm
__forceinline vec3 log(vec3 v)
{
    return vec3(logf(v.x), logf(v.y), logf(v.z));
}

/// Computes the base 2 logarithm
__forceinline vec3 log2(vec3 v)
{
    return vec3(log2f(v.x), log2f(v.y), log2f(v.z));
}

/// Computes the base 10 logarithm
__forceinline vec3 log10(vec3 v)
{
    return vec3(log10f(v.x), log10f(v.y), log10f(v.z));
}

/// Computes the value of base raised to the power exponent
__forceinline vec3 pow(vec3 a, vec3 b)
{
    return vec3(powf(a.x, b.x), powf(a.y, b.y), powf(a.z, b.z));
}

/// Get the fractal part of floating point
__forceinline vec3 frac(vec3 v)
{
    return vec3(fracf(v.x), fracf(v.y), fracf(v.z));
}

/// Computes the floating-point remainder of the division operation x/y
__forceinline vec3 fmod(vec3 a, vec3 b)
{
    return vec3(fmodf(a.x, b.x), fmodf(a.y, b.y), fmodf(a.z, b.z));
}

/// Computes the smallest integer value not less than 'x'
__forceinline vec3 ceil(vec3 v)
{
    return vec3(ceilf(v.x), ceilf(v.y), ceilf(v.z));
}

/// Computes the largest integer value not greater than 'x'
__forceinline vec3 floor(vec3 v)
{
    return vec3(floorf(v.x), floorf(v.y), floorf(v.z));
}

/// Computes the nearest integer value
__forceinline vec3 round(vec3 v)
{
    return vec3(roundf(v.x), roundf(v.y), roundf(v.z));
}

/// Computes the nearest integer not greater in magnitude than 'x'
__forceinline vec3 trunc(vec3 v)
{
    return vec3(truncf(v.x), truncf(v.y), truncf(v.z));
}

/// Get the smaller value
__forceinline vec3 min(vec3 a, vec3 b)
{
    return vec3(minf(a.x, b.x), minf(a.y, b.y), minf(a.z, b.z));
}

/// Get the larger value
__forceinline vec3 max(vec3 a, vec3 b)
{
    return vec3(maxf(a.x, b.x), maxf(a.y, b.y), maxf(a.z, b.z));
}

/// Clamps the 'x' value to the [min, max].
__forceinline vec3 clamp(vec3 v, vec3 min, vec3 max)
{
    return vec3(clampf(v.x, min.x, max.x), clampf(v.y, min.y, max.y), clampf(v.z, min.z, max.z));
}

/// Clamps the specified value within the range of 0 to 1
__forceinline vec3 saturate(vec3 v)
{
    return vec3(saturatef(v.x), saturatef(v.y), saturatef(v.z));
}

/// Compares two values, returning 0 or 1 based on which value is greater.
__forceinline vec3 step(vec3 a, vec3 b)
{
    return vec3(stepf(a.x, b.x), stepf(a.y, b.y), stepf(a.z, b.z));
}

/// Performs a linear interpolation.
__forceinline vec3 lerp(vec3 a, vec3 b, vec3 t)
{
    return vec3(lerpf(a.x, b.x, t.x), lerpf(a.y, b.y, t.y), lerpf(a.z, b.z, t.z));
}

/// Performs a linear interpolation.
__forceinline vec3 lerp(vec3 a, vec3 b, float t)
{
    return vec3(lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t));
}

// Compute a smooth Hermite interpolation
__forceinline vec3 smoothstep(vec3 a, vec3 b, vec3 t)
{
    return vec3(smoothstepf(a.x, b.x, t.x), smoothstepf(a.y, b.y, t.y), smoothstepf(a.z, b.z, t.z));
}

/// Computes square root of 'x'.
__forceinline vec3 sqrt(vec3 v)
{
    return vec3(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z));
}

/// Computes inverse square root of 'x'.
__forceinline vec3 rsqrt(vec3 v)
{
    return vec3(rsqrtf(v.x), rsqrtf(v.y), rsqrtf(v.z));
}

/// Compute cross product of two vectors
__forceinline vec3 cross(vec3 a, vec3 b)
{
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

/// Compute dot product of two vectors
__forceinline float dot(vec3 a, vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// Compute squared length of vector
__forceinline float lensqr(vec3 v)
{
    return dot(v, v);
}

/// Compute length of vector
__forceinline float length(vec3 v)
{
    return sqrtf(lensqr(v));
}

/// Compute distance from 'a' to b
__forceinline float distance(vec3 a, vec3 b)
{
    return length(a - b);
}

/// Compute squared distance from 'a' to b
__forceinline float distsqr(vec3 a, vec3 b)
{
    return lensqr(a - b);
}

/// Compute normalized vector
__forceinline vec3 normalize(vec3 v)
{
    const float lsqr = lensqr(v);
    if (lsqr > 0.0f)
    {
        const float f = rsqrtf(lsqr);
        return vec3(v.x * f, v.y * f, v.z * f);
    }
    else
    {
        return v;
    }
}

/// Compute reflection vector
__forceinline vec3 reflect(vec3 v, vec3 n)
{
    return v - 2.0f * dot(v, n) * n;
}

/// Compute refraction vector
__forceinline vec3 refract(vec3 v, vec3 n, float eta)
{
    const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
    return k < 0.0f
        ? vec3(0.0f)
        : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
}

/// Compute faceforward vector
__forceinline vec3 faceforward(vec3 n, vec3 i, vec3 nref)
{
    return dot(i, nref) < 0.0f ? n : -n;
}

/// Computes sign of 'x'
__forceinline ivec4 sign(vec4 v)
{
    return ivec4{ signf(v.x), signf(v.y), signf(v.z), signf(v.w) };
}

/// Computes absolute value
__forceinline vec4 abs(vec4 v)
{
    return vec4(fabsf(v.x), fabsf(v.y), fabsf(v.z), fabsf(v.w));
}

/// Computes cosine
__forceinline vec4 cos(vec4 v)
{
    return vec4(cosf(v.x), cosf(v.y), cosf(v.z), cosf(v.w));
}

/// Computes sine
__forceinline vec4 sin(vec4 v)
{
    return vec4(sinf(v.x), sinf(v.y), sinf(v.z), sinf(v.w));
}

/// Computes tangent
__forceinline vec4 tan(vec4 v)
{
    return vec4(tanf(v.x), tanf(v.y), tanf(v.z), tanf(v.w));
}

/// Computes hyperbolic cosine
__forceinline vec4 cosh(vec4 v)
{
    return vec4(coshf(v.x), coshf(v.y), coshf(v.z), coshf(v.w));
}

/// Computes hyperbolic sine
__forceinline vec4 sinh(vec4 v)
{
    return vec4(sinhf(v.x), sinhf(v.y), sinhf(v.z), sinhf(v.w));
}

/// Computes hyperbolic tangent
__forceinline vec4 tanh(vec4 v)
{
    return vec4(tanhf(v.x), tanhf(v.y), tanhf(v.z), tanhf(v.w));
}

/// Computes inverse cosine
__forceinline vec4 acos(vec4 v)
{
    return vec4(acosf(v.x), acosf(v.y), acosf(v.z), acosf(v.w));
}

/// Computes inverse sine
__forceinline vec4 asin(vec4 v)
{
    return vec4(asinf(v.x), asinf(v.y), asinf(v.z), asinf(v.w));
}

/// Computes inverse tangent
__forceinline vec4 atan(vec4 v)
{
    return vec4(atanf(v.x), atanf(v.y), atanf(v.z), atanf(v.w));
}

/// Computes inverse tangent with 2 args
__forceinline vec4 atan2(vec4 a, vec4 b)
{
    return vec4(atan2f(a.x, b.x), atan2f(a.y, b.y), atan2f(a.z, b.z), atan2f(a.w, b.w));
}

/// Computes Euler number raised to the power 'x'
__forceinline vec4 exp(vec4 v)
{
    return vec4(expf(v.x), expf(v.y), expf(v.z), expf(v.w));
}

/// Computes 2 raised to the power 'x'
__forceinline vec4 exp2(vec4 v)
{
    return vec4(exp2f(v.x), exp2f(v.y), exp2f(v.z), exp2f(v.w));
}

/// Computes the base Euler number logarithm
__forceinline vec4 log(vec4 v)
{
    return vec4(logf(v.x), logf(v.y), logf(v.z), logf(v.w));
}

/// Computes the base 2 logarithm
__forceinline vec4 log2(vec4 v)
{
    return vec4(log2f(v.x), log2f(v.y), log2f(v.z), log2f(v.w));
}

/// Computes the base 10 logarithm
__forceinline vec4 log10(vec4 v)
{
    return vec4(log10f(v.x), log10f(v.y), log10f(v.z), log10f(v.w));
}

/// Computes the value of base raised to the power exponent
__forceinline vec4 pow(vec4 a, vec4 b)
{
    return vec4(powf(a.x, b.x), powf(a.y, b.y), powf(a.z, b.z), powf(a.w, b.w));
}

/// Get the fractal part of floating point
__forceinline vec4 frac(vec4 v)
{
    return vec4(fracf(v.x), fracf(v.y), fracf(v.z), fracf(v.w));
}

/// Computes the floating-point remainder of the division operation x/y
__forceinline vec4 fmod(vec4 a, vec4 b)
{
    return vec4(fmodf(a.x, b.x), fmodf(a.y, b.y), fmodf(a.z, b.z), fmodf(a.w, b.w));
}

/// Computes the smallest integer value not less than 'x'
__forceinline vec4 ceil(vec4 v)
{
    return vec4(ceilf(v.x), ceilf(v.y), ceilf(v.z), ceilf(v.w));
}

/// Computes the largest integer value not greater than 'x'
__forceinline vec4 floor(vec4 v)
{
    return vec4(floorf(v.x), floorf(v.y), floorf(v.z), floorf(v.w));
}

/// Computes the nearest integer value
__forceinline vec4 round(vec4 v)
{
    return vec4(roundf(v.x), roundf(v.y), roundf(v.z), roundf(v.w));
}

/// Computes the nearest integer not greater in magnitude than 'x'
__forceinline vec4 trunc(vec4 v)
{
    return vec4(truncf(v.x), truncf(v.y), truncf(v.z), truncf(v.w));
}

/// Get the smaller value
__forceinline vec4 min(vec4 a, vec4 b)
{
    return vec4(
        minf(a.x, b.x),
        minf(a.y, b.y),
        minf(a.z, b.z),
        minf(a.w, b.w)
    );
}

/// Get the larger value
__forceinline vec4 max(vec4 a, vec4 b)
{
    return vec4(
        maxf(a.x, b.x), 
        maxf(a.y, b.y),
        maxf(a.z, b.z),
        maxf(a.w, b.w)
    );
}

/// Clamps the 'x' value to the [min, max].
__forceinline vec4 clamp(vec4 v, vec4 min, vec4 max)
{
    return vec4(
        clampf(v.x, min.x, max.x), 
        clampf(v.y, min.y, max.y), 
        clampf(v.z, min.z, max.z), 
        clampf(v.w, min.w, max.w)
    );
}

/// Clamps the specified value within the range of 0 to 1
__forceinline vec4 saturate(vec4 v)
{
    return vec4(saturatef(v.x), saturatef(v.y), saturatef(v.z), saturatef(v.w));
}

/// Compares two values, returning 0 or 1 based on which value is greater.
__forceinline vec4 step(vec4 a, vec4 b)
{
    return vec4(
        stepf(a.x, b.x),
        stepf(a.y, b.y),
        stepf(a.z, b.z),
        stepf(a.w, b.w)
    );
}

/// Performs a linear interpolation.
__forceinline vec4 lerp(vec4 a, vec4 b, vec4 t)
{
    return vec4(
        lerpf(a.x, b.x, t.x),
        lerpf(a.y, b.y, t.y),
        lerpf(a.z, b.z, t.z),
        lerpf(a.w, b.w, t.w)
    );
}

/// Performs a linear interpolation.
__forceinline vec4 lerp(vec4 a, vec4 b, float t)
{
    return vec4(
        lerpf(a.x, b.x, t),
        lerpf(a.y, b.y, t),
        lerpf(a.z, b.z, t),
        lerpf(a.w, b.w, t)
    );
}

/// Compute a smooth Hermite interpolation
__forceinline vec4 smoothstep(vec4 a, vec4 b, vec4 t)
{
    return vec4(
        smoothstepf(a.x, b.x, t.x),
        smoothstepf(a.y, b.y, t.y),
        smoothstepf(a.z, b.z, t.z),
        smoothstepf(a.w, b.w, t.w)
    );
}

/// Computes square root of 'x'.
__forceinline vec4 sqrt(vec4 v)
{
    return vec4(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w));
}

/// Computes inverse square root of 'x'.
__forceinline vec4 rsqrt(vec4 v)
{
    return vec4(rsqrtf(v.x), rsqrtf(v.y), rsqrtf(v.z), rsqrtf(v.w));
}

/// Compute dot product of two vectors
__forceinline float dot(vec4 a, vec4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/// Compute squared length of vector
__forceinline float lensqr(vec4 v)
{
    return dot(v, v);
}

/// Compute length of vector
__forceinline float length(vec4 v)
{
    return sqrtf(lensqr(v));
}

/// Compute distance from 'a' to b
__forceinline float distance(vec4 a, vec4 b)
{
    return length(a - b);
}

/// Compute squared distance from 'a' to b
__forceinline float distsqr(vec4 a, vec4 b)
{
    return lensqr(a - b);
}

/// Compute normalized vector
__forceinline vec4 normalize(vec4 v)
{
    const float lsqr = lensqr(v);
    if (lsqr > 0.0f)
    {
        const float f = rsqrtf(lsqr);
        return vec4(v.x * f, v.y * f, v.z * f, v.w * f);
    }
    else
    {
        return v;
    }
}

/// Compute reflection vector
__forceinline vec4 reflect(vec4 v, vec4 n)
{
    return v - 2.0f * dot(v, n) * n;
}

/// Compute refraction vector
__forceinline vec4 refract(vec4 v, vec4 n, float eta)
{
    const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
    return k < 0.0f
        ? vec4(0.0f)
        : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
}

/// Compute faceforward vector
__forceinline vec4 faceforward(vec4 n, vec4 i, vec4 nref)
{
    return dot(i, nref) < 0.0f ? n : -n;
}

/// Quaternion multiplication
__forceinline vec4 qmul(vec4 a, vec4 b)
{
    const vec3  a3  = vec3(a.x, a.y, a.z);
    const vec3  b3  = vec3(b.x, b.y, b.z);

    const vec3  xyz = a3 * b.w + b3 * a.w + cross(a3, b3);
    const float w   = a.w * b.w - dot(a3, b3);
    
    return vec4(xyz, w);
}

__forceinline vec4 qinverse(vec4 q)
{
    return vec4(q.x, q.y, q.z, -q.w);
}

__forceinline vec4 qconj(vec4 q)
{
    return vec4(-q.x, -q.y, -q.z, q.w);
}

__forceinline vec4 quatFromAxisAngle(vec3 axis, float angle)
{
    if (lensqr(axis) == 0.0f)
    {
        return vec4(0, 0, 0, 1);
    }

    return vec4(normalize(axis) * sinf(angle * 0.5f), cosf(angle * 0.5f));
}

__forceinline vec4 quatToAxisAngle(vec4 quat)
{
    vec4 c = quat;
    if (c.w != 0.0f)
    {
        c = normalize(quat);
    }

    const float den = sqrtf(1.0f - c.w * c.w);
    const vec3 axis = (den > 0.0001f) 
        ? vec3(c.x, c.y, c.z) / den
        : vec3(1, 0, 0);

    const float angle = 2.0f * cosf(c.w);
    return vec4(axis, angle);
}

__forceinline void quatToAxisAngle(vec4 quat, vec3* axis, float* angle)
{
    vec4 axisAngle = quatToAxisAngle(quat);
    if (axis) *axis = (vec3)axisAngle;
    if (angle) *angle = axisAngle.w;
}

__forceinline vec4 quatFromEuler(float x, float y, float z)
{
    float r;
    float p;

    r = z * 0.5f;
    p = x * 0.5f;
    y = y * 0.5f; // Now y mean yaw

    const float c1 = cosf(y);
    const float c2 = cosf(p);
    const float c3 = cosf(r);
    const float s1 = sinf(y);
    const float s2 = sinf(p);
    const float s3 = sinf(r);

    return vec4(
        s1 * s2 * c3 + c1 * c2 * s3,
        s1 * c2 * c3 + c1 * s2 * s3,
        c1 * s2 * c3 - s1 * c2 * s3,
        c1 * c2 * c3 - s1 * s2 * s3
    );
}

/// Computes absolute value
__forceinline mat4 abs(mat4 m)
{
    return mat4(abs(m.row0), abs(m.row1), abs(m.row2), abs(m.row3));
}

/// Computes cosine
__forceinline mat4 cos(mat4 m)
{
    return mat4(
        cos(m.row0),
        cos(m.row1),
        cos(m.row2),
        cos(m.row3)
    );
}

/// Computes sine
__forceinline mat4 sin(mat4 m)
{
    return mat4(
        sin(m.row0),
        sin(m.row1),
        sin(m.row2),
        sin(m.row3)
    );
}

/// Computes tangent
__forceinline mat4 tan(mat4 m)
{
    return mat4(
        tan(m.row0),
        tan(m.row1),
        tan(m.row2),
        tan(m.row3)
    );
}

/// Computes hyperbolic cosine
__forceinline mat4 cosh(mat4 m)
{
    return mat4(
        cosh(m.row0),
        cosh(m.row1),
        cosh(m.row2),
        cosh(m.row3)
    );
}

/// Computes hyperbolic sine
__forceinline mat4 sinh(mat4 m)
{
    return mat4(
        sinh(m.row0),
        sinh(m.row1),
        sinh(m.row2),
        sinh(m.row3)
    );
}

/// Computes hyperbolic tangent
__forceinline mat4 tanh(mat4 m)
{
    return mat4(
        tanh(m.row0),
        tanh(m.row1),
        tanh(m.row2),
        tanh(m.row3)
    );
}

/// Computes inverse cosine
__forceinline mat4 acos(mat4 m)
{
    return mat4(
        acos(m.row0),
        acos(m.row1),
        acos(m.row2),
        acos(m.row3)
    );
}

/// Computes inverse sine
__forceinline mat4 asin(mat4 m)
{
    return mat4(
        asin(m.row0),
        asin(m.row1),
        asin(m.row2),
        asin(m.row3)
    );
}

/// Computes inverse tangent
__forceinline mat4 atan(mat4 m)
{
    return mat4(
        atan(m.row0),
        atan(m.row1),
        atan(m.row2),
        atan(m.row3)
    );
}

/// Computes inverse tangent with 2 args
__forceinline mat4 atan2(mat4 a, mat4 b)
{
    return mat4(
        atan2(a.row0, b.row0),
        atan2(a.row1, b.row1),
        atan2(a.row2, b.row2),
        atan2(a.row3, b.row3)
    );
}

/// Computes Euler number raised to the power 'x'
__forceinline mat4 exp(mat4 m)
{
    return mat4(
        exp(m.row0),
        exp(m.row1),
        exp(m.row2),
        exp(m.row3)
    );
}

/// Computes 2 raised to the power 'x'
__forceinline mat4 exp2(mat4 m)
{
    return mat4(
        exp2(m.row0),
        exp2(m.row1),
        exp2(m.row2),
        exp2(m.row3)
    );
}

/// Computes the base Euler number logarithm
__forceinline mat4 log(mat4 m)
{
    return mat4(
        log(m.row0),
        log(m.row1),
        log(m.row2),
        log(m.row3)
    );
}

/// Computes the base 2 logarithm
__forceinline mat4 log2(mat4 m)
{
    return mat4(
        log2(m.row0),
        log2(m.row1),
        log2(m.row2),
        log2(m.row3)
    );
}

/// Computes the base 10 logarithm
__forceinline mat4 log10(mat4 m)
{
    return mat4(
        log10(m.row0),
        log10(m.row1),
        log10(m.row2),
        log10(m.row3)
    );
}

/// Computes the value of base raised to the power exponent
__forceinline mat4 pow(mat4 a, mat4 b)
{
    return mat4(
        pow(a.row0, b.row0),
        pow(a.row1, b.row1),
        pow(a.row2, b.row2),
        pow(a.row3, b.row3)
    );
}

/// Get the fractal part of floating point
__forceinline mat4 frac(mat4 m)
{
    return mat4(
        frac(m.row0),
        frac(m.row1),
        frac(m.row2),
        frac(m.row3)
    );
}

/// Computes the floating-point remainder of the division operation x/y
__forceinline mat4 fmod(mat4 a, mat4 b)
{
    return mat4(
        fmod(a.row0, b.row0),
        fmod(a.row1, b.row1),
        fmod(a.row2, b.row2),
        fmod(a.row3, b.row3)
    );
}

/// Computes the smallest integer value not less than 'x'
__forceinline mat4 ceil(mat4 m)
{
    return mat4(
        ceil(m.row0),
        ceil(m.row1),
        ceil(m.row2),
        ceil(m.row3)
    );
}

/// Computes the largest integer value not greater than 'x'
__forceinline mat4 floor(mat4 m)
{
    return mat4(
        floor(m.row0),
        floor(m.row1),
        floor(m.row2),
        floor(m.row3)
    );
}

/// Computes the nearest integer value
__forceinline mat4 round(mat4 m)
{
    return mat4(
        round(m.row0),
        round(m.row1),
        round(m.row2),
        round(m.row3)
    );
}

/// Computes the nearest integer not greater in magnitude than 'x'
__forceinline mat4 trunc(mat4 m)
{
    return mat4(
        trunc(m.row0),
        trunc(m.row1),
        trunc(m.row2),
        trunc(m.row3)
    );
}

/// Get the smaller value
__forceinline mat4 min(mat4 a, mat4 b)
{
    return mat4(
        min(a.row0, b.row0),
        min(a.row1, b.row1),
        min(a.row2, b.row2),
        min(a.row3, b.row3)
    );
}

/// Get the larger value
__forceinline mat4 max(mat4 a, mat4 b)
{
    return mat4(
        max(a.row0, b.row0),
        max(a.row1, b.row1),
        max(a.row2, b.row2),
        max(a.row3, b.row3)
    );
}

/// Clamps the 'x' value to the [min, max].
__forceinline mat4 clamp(mat4 v, mat4 min, mat4 max)
{
    return mat4(
        clamp(v.row0, min.row0, max.row0),
        clamp(v.row1, min.row1, max.row1),
        clamp(v.row2, min.row2, max.row2),
        clamp(v.row3, min.row3, max.row3)
    );
}

/// Clamps the specified value within the range of 0 to 1
__forceinline mat4 saturate(mat4 m)
{
    return mat4(
        saturate(m.row0),
        saturate(m.row1),
        saturate(m.row2),
        saturate(m.row3)
    );
}

/// Compares two values, returning 0 or 1 based on which value is greater.
__forceinline mat4 step(mat4 a, mat4 b)
{
    return mat4(
        step(a.row0, b.row0),
        step(a.row1, b.row1),
        step(a.row2, b.row2),
        step(a.row3, b.row3)
    );
}

/// Performs a linear interpolation.
__forceinline mat4 lerp(mat4 a, mat4 b, mat4 t)
{
    return mat4(
        lerp(a.row0, b.row0, t.row0),
        lerp(a.row1, b.row1, t.row1),
        lerp(a.row2, b.row2, t.row2),
        lerp(a.row3, b.row3, t.row3)
    );
}

/// Performs a linear interpolation.
__forceinline mat4 lerp(mat4 a, mat4 b, float t)
{
    return mat4(
        lerp(a.row0, b.row0, t),
        lerp(a.row1, b.row1, t),
        lerp(a.row2, b.row2, t),
        lerp(a.row3, b.row3, t)
    );
}

/// Compute a smooth Hermite interpolation
__forceinline mat4 smoothstep(mat4 a, mat4 b, mat4 t)
{
    return mat4(
        smoothstep(a.row0, b.row0, t.row0),
        smoothstep(a.row1, b.row1, t.row1),
        smoothstep(a.row2, b.row2, t.row2),
        smoothstep(a.row3, b.row3, t.row3)
    );
}

/// Computes square root of 'x'.
__forceinline mat4 sqrt(mat4 m)
{
    return mat4(sqrt(m.row0), sqrt(m.row1), sqrt(m.row2), sqrt(m.row3));
}

/// Computes inverse square root of 'x'.
__forceinline mat4 rsqrt(mat4 m)
{
    return mat4(rsqrt(m.row0), rsqrt(m.row1), rsqrt(m.row2), rsqrt(m.row3));
}

__forceinline vec4 mul(mat4 m, vec4 v)
{
    return vec4(
        m.row0.x * v.x + m.row1.x * v.y + m.row2.x * v.z + m.row3.x * v.w,
        m.row0.y * v.x + m.row1.y * v.y + m.row2.y * v.z + m.row3.y * v.w,
        m.row0.z * v.x + m.row1.z * v.y + m.row2.z * v.z + m.row3.z * v.w,
        m.row0.w * v.x + m.row1.w * v.y + m.row2.w * v.z + m.row3.w * v.w
    );
}

__forceinline vec4 mul(vec4 v, mat4 m)
{
    return vec4(
        dot(v, m.row0),
        dot(v, m.row1),
        dot(v, m.row2),
        dot(v, m.row3)
    );
}

__forceinline vec3 mul(mat4 a, vec3 b)
{
    const vec4 b0 = vec4(b.x, b.y, b.z, 1.0f);
    const vec4 b1 = mul(a, b0);

    const float iw = 1.0f / b1.w;
    return vec3(b1.x * iw, b1.y * iw, b1.z * iw);
}

__forceinline vec3 mul(vec3 a, mat4 b)
{
    const vec4 a0 = vec4(a.x, a.y, a.z, 1.0f);
    const vec4 a1 = mul(a0, b);

    const float iw = 1.0f / a1.w;
    return vec3(a1.x * iw, a1.y * iw, a1.z * iw);
}

__forceinline mat4 mul(mat4 a, mat4 b)
{
    return mat4(
        mul(a, b.row0),
        mul(a, b.row1),
        mul(a, b.row2),
        mul(a, b.row3)
    );
}

__forceinline mat4 transpose(mat4 m)
{
    mat4 result;
    return mat4(
        m.m00, m.m10, m.m20, m.m30,
        m.m01, m.m11, m.m21, m.m31,
        m.m02, m.m12, m.m22, m.m32,
        m.m03, m.m13, m.m23, m.m33
    );
}

__forceinline mat4 inverse(mat4 m)
{
    const float n11 = m.m00, n12 = m.m10, n13 = m.m20, n14 = m.m30;
    const float n21 = m.m01, n22 = m.m11, n23 = m.m21, n24 = m.m31;
    const float n31 = m.m02, n32 = m.m12, n33 = m.m22, n34 = m.m32;
    const float n41 = m.m03, n42 = m.m13, n43 = m.m23, n44 = m.m33;

    const float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    const float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    const float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    const float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    const float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    if (det < 0.0f)
    {
        return mat4(0.0f);
    }

    const float idet = 1.0f / det;
    mat4 result;

    result.m00 = t11 * idet;
    result.m01 = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    result.m02 = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    result.m03 = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    result.m10 = t12 * idet;
    result.m11 = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    result.m12 = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    result.m13 = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    result.m20 = t13 * idet;
    result.m21 = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    result.m22 = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    result.m23 = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    result.m30 = t14 * idet;
    result.m31 = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    result.m32 = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    result.m33 = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return result;
}

__forceinline mat4 mat4Identity()
{
    return mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

__forceinline mat4 mat4Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    const float sum_rl = (right + left);
    const float sum_tb = (top + bottom);
    const float sum_nf = (zNear + zFar);
    const float inv_rl = (1.0f / (right - left));
    const float inv_tb = (1.0f / (top - bottom));
    const float inv_nf = (1.0f / (zFar - zNear));

    return mat4(
         (inv_rl + inv_rl),               0.0f,               0.0f, 0.0f,
                      0.0f,  (inv_tb + inv_tb),               0.0f, 0.0f,
                      0.0f,               0.0f,  (inv_nf + inv_nf), 0.0f,
        -(sum_rl * inv_rl), -(sum_tb * inv_tb), -(sum_nf * inv_nf), 1.0f
    );
}

__forceinline mat4 mat4Frustum(float l, float r, float b, float t, float n, float f)
{
    const float x = 1.0f / (r - l);
    const float y = 1.0f / (t - b);
    const float z = 1.0f / (f - n);

    return mat4(
        2.0f * x, 0, 0, 0,
        0, 2.0f * y, 0, 0,
        x * (l + r), y * (b + t), z * (n + f), 1.0f,
        0, 0, 2.0f * z, 0
    );
}

__forceinline mat4 mat4Perspective(float fovRadians, float aspect, float znear, float zfar)
{
    const float zoomX = 1.0f / tanf(fovRadians * 0.5f);
    const float zoomY = zoomX * aspect;
    
    const float rangeInv   = 1.0f / (znear - zfar);
    const float zClipBias0 = (znear + zfar) * rangeInv;
    const float zClipBias1 = (2.0f * znear * zfar) * rangeInv;

    return mat4(
        vec4(zoomX,  0.0f,       0.0f,  0.0f),
        vec4( 0.0f, zoomY,       0.0f,  0.0f),
        vec4( 0.0f,  0.0f, zClipBias0, -1.0f),
        vec4( 0.0f,  0.0f, zClipBias1,  0.0f)
    );
}

__forceinline mat4 mat4LookAt(vec3 eye, vec3 target, vec3 up)
{
    const vec3 z = normalize(eye - target);
    const vec3 x = normalize(cross(normalize(up), z));
    const vec3 y = normalize(cross(z, x));

    return mat4(
         x.x,  y.x,  z.x, -dot(x, eye),
         x.y,  y.y,  z.y, -dot(y, eye),
         x.z,  y.z,  z.z, -dot(z, eye),
        0.0f, 0.0f, 0.0f,         1.0f
    );
}

__forceinline mat4 mat4Scalation(float x, float y, float z = 1.0f)
{
    return mat4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    );
}

__forceinline mat4 mat4Scalation(float s)
{
    return mat4Scalation(s, s, s);
}

__forceinline mat4 mat4Scalation(vec2 v)
{
    return mat4Scalation(v.x, v.y, 1.0f);
}

__forceinline mat4 mat4Scalation(vec3 v)
{
    return mat4Scalation(v.x, v.y, v.z);
}

__forceinline mat4 mat4Translation(float x, float y, float z = 0.0f)
{
    return mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    );
}

__forceinline mat4 mat4Translation(vec2 v)
{
    return mat4Translation(v.x, v.y);
}

__forceinline mat4 mat4Translation(vec3 v)
{
    return mat4Translation(v.x, v.y, v.z);
}

__forceinline mat4 mat4Rotation(float x, float y, float z, float radians)
{
    const float c = cosf(-radians);
    const float s = sinf(-radians);
    const float t = 1.0f - c;

    return mat4(
        /* Row 0 */
        t * x * x + c,
        t * x * y - s * z,
        t * x * z + s * y,
        0.0f,

        /* Row 1 */
        t* x* y + s * z,
        t* y* y + c,
        t* y* z - s * x,
        0.0f,

        /* Row 2 */
        t* x* z - s * y,
        t* y* z + s * x,
        t* z* z + c,
        0.0f,

        /* Row 3 */
        0, 0, 0, 1.0f
    );
}

__forceinline mat4 mat4Rotation(vec3 axis, float angle)
{
    return mat4Rotation(axis.x, axis.y, axis.z, angle);
}

__forceinline mat4 mat4RotationX(float angle)
{
    const float s = sinf(angle);
    const float c = cosf(angle);

    return mat4(
        1,  0, 0, 0,
        0,  c, s, 0,
        0, -s, c, 0,
        0,  0, 0, 1
    );
}

__forceinline mat4 mat4RotationY(float angle)
{
    const float s = sinf(angle);
    const float c = cosf(angle);

    return mat4(
         c, 0, s, 0,
         0, 1, 0, 0,
        -s, 0, c, 0,
         0, 0, 0, 1
    );
}

__forceinline mat4 mat4RotationZ(float radians)
{
    const float s = sinf(radians);
    const float c = cosf(radians);

    return mat4(
         c, s, 0, 0,
        -s, c, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1
    );
}

__forceinline mat4 mat4_rotation(vec4 quaternion)
{
    vec4 axisangle = quat_to_axis_angle(quaternion);
    return mat4_rotation(axisangle.x, axisangle.y, axisangle.z, axisangle.w);
}

__forceinline void mat4_decompose(mat4 m, vec3* scalation, vec4* quaternion, vec3* translation)
{
    if (translation)
    {
        *translation = vec3(m.row3);
    }

    if (!scalation && !quaternion)
    {
        return;
    }

    vec3 xaxis = vec3(m.row0);
    vec3 yaxis = vec3(m.row1);
    vec3 zaxis = vec3(m.row2);

    float scale_x = vec3_length(xaxis);
    float scale_y = vec3_length(yaxis);
    float scale_z = vec3_length(zaxis);

    const float n11 = m.m00, n12 = m.m10, n13 = m.m20, n14 = m.m30;
    const float n21 = m.m01, n22 = m.m11, n23 = m.m21, n24 = m.m31;
    const float n31 = m.m02, n32 = m.m12, n33 = m.m22, n34 = m.m32;
    const float n41 = m.m03, n42 = m.m13, n43 = m.m23, n44 = m.m33;

    const float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    const float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    const float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    const float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    const float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    if (det < 0) scale_z = -scale_z;

    if (scalation)
    {
        *scalation = vec3(scale_x, scale_y, scale_z);
    }

    if (!quaternion)
    {
        return;
    }

    float rn;

    // Factor the scale out of the matrix axes.
    rn = 1.0f / scale_x;
    xaxis.x *= rn;
    xaxis.y *= rn;
    xaxis.z *= rn;

    rn = 1.0f / scale_y;
    yaxis.x *= rn;
    yaxis.y *= rn;
    yaxis.z *= rn;

    rn = 1.0f / scale_z;
    zaxis.x *= rn;
    zaxis.y *= rn;
    zaxis.z *= rn;

    // Now calculate the rotation from the resulting matrix (axes).
    float trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

    if (trace > 0.0001f)
    {
        float s = 0.5f / sqrtf(trace);
        quaternion->w = 0.25f / s;
        quaternion->x = (yaxis.z - zaxis.y) * s;
        quaternion->y = (zaxis.x - xaxis.z) * s;
        quaternion->z = (xaxis.y - yaxis.x) * s;
    }
    else
    {
        // Note: since X axis, Y axis, and Z axis are normalized, 
        // we will never divide by zero in the code below.
        if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
        {
            float s = 0.5f / sqrtf(1.0f + xaxis.x - yaxis.y - zaxis.z);
            quaternion->w = (yaxis.z - zaxis.y) * s;
            quaternion->x = 0.25f / s;
            quaternion->y = (yaxis.x + xaxis.y) * s;
            quaternion->z = (zaxis.x + xaxis.z) * s;
        }
        else if (yaxis.y > zaxis.z)
        {
            float s = 0.5f / sqrtf(1.0f + yaxis.y - xaxis.x - zaxis.z);
            quaternion->w = (zaxis.x - xaxis.z) * s;
            quaternion->x = (yaxis.x + xaxis.y) * s;
            quaternion->y = 0.25f / s;
            quaternion->z = (zaxis.y + yaxis.z) * s;
        }
        else
        {
            float s = 0.5f / sqrtf(1.0f + zaxis.z - xaxis.x - yaxis.y);
            quaternion->w = (xaxis.y - yaxis.x) * s;
            quaternion->x = (zaxis.x + xaxis.z) * s;
            quaternion->y = (zaxis.y + yaxis.z) * s;
            quaternion->z = 0.25f / s;
        }
    }
}

__forceinline void mat4_decompose(mat4 m, vec3* scalation, vec3* axis, float* angle, vec3* translation)
{
    if (axis || angle)
    {
        vec4 quat;
        mat4_decompose(m, scalation, &quat, translation);

        quat_to_axis_angle(quat, axis, angle);
    }
    else
    {
        mat4_decompose(m, scalation, (vec4*)0, translation);
    }
}
