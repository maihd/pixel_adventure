#pragma once

// -------------------------------------------------------------
// Function call conventions
// -------------------------------------------------------------

#if defined(__GNUC__)
#   define __vectorcall  /* NO VECTORCALL SUPPORTED */
#   define __forceinline static __attribute__((always_inline))
#endif

// -------------------------------------------------------------
// Data structure alignment
// -------------------------------------------------------------

#ifndef ALIGNAS
#   if defined(_MSC_VER)
#       define ALIGNAS(TYPE_NAME, ALIGNMENT) __declspec(align(ALIGNMENT)) TYPE_NAME
#   elif defined(__GNUC__)
#       define ALIGNAS(TYPE_NAME, ALIGNMENT) TYPE_NAME __attribute__((aligned(ALIGNMENT)))
#   else
#       define ALIGNAS(TYPE_NAME, ALIGNMENT) alignas(ALIGNMENT) TYPE_NAME
#   endif
#endif

#ifndef ALIGNOF
#   if defined(_MSC_VER)
#       define ALIGNOF(X) __alignof(X)
#   elif defined(__GNUC__)
#       define ALIGNOF(X) __alignof__(typeof(X))
#   else
#       define ALIGNOF(X) alignof(decltype(X))
#   endif
#endif

// -------------------------------------------------------------
// MISC
// -------------------------------------------------------------

// constexpr is helpful
#ifndef __cplusplus && !defined(constexpr)
#   define constexpr static const
#endif
