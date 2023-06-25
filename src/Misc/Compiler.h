#pragma once

// -------------------------------------------------------------
// Function call conventions
// -------------------------------------------------------------

#if defined(__GNUC__)
#   define __vectorcall  /* NO VECTORCALL SUPPORTED */
#   define __forceinline static __attribute__((always_inline))
#endif

#if !defined(__cplusplus) && !defined(constexpr)
#define constexpr static const
#endif

#if !defined(__cplusplus) && !defined(nullptr)
#define nullptr ((void*)0)
#endif

#if !defined(__enum_type)
#if defined(__cplusplus)
#define __enum_type(T) : T
#else
#define __enum_type(T)
#endif
#endif

#if !defined(__name_of)
#define __name_of(T) #T
#endif

#if !defined(__default_init)
#ifdef __cplusplus
#define __default_init(...) = ##__VA_ARGS__
#else
#define __default_init(...)
#endif
#endif

// assert_type_is_memcopyable();
// assert_type_is_trivial();
// assert_type_is_pod();
// assert_type_size();
// assert_type_alignment();
// assert_not_null();
// assert_in_range();
// assert_enough_memory();

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
