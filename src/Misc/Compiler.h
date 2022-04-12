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

#ifndef ALIGNOF
#   if defined(__cplusplus)
#       define ALIGNOF(X) alignof(decltype(X))
#   elif defined(__GNUC__)
#       define ALIGNOF(X) __alignof__(typeof(X))
#   elif defined(_MSC_VER)
#       define ALIGNOF(X) __alignof(X)
#   else
#       error The compiler is not support alignof operator.
#   endif
#endif

// -------------------------------------------------------------
// MISC
// -------------------------------------------------------------

// constexpr is helpful
#ifndef __cplusplus && !defined(constexpr)
#   define constexpr static const
#endif

// assert_type_is_memcopyable();
// assert_type_is_trivial();
// assert_type_is_pod();
// assert_type_size();
// assert_type_alignment();
// assert_not_null();
// assert_in_range();
// assert_enough_memory();