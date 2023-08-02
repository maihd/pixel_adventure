#pragma once

#if defined(__GNUC__) || defined(__clang__)
#   include <stdint.h>
#   if defined(__GNUC__) && (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= 40800
#       define Atomic_GetI32(variable)          ({ int32_t result; __atomic_load(variable, result, __ATOMIC_RELAXED); result; })
#       define Atomic_SetI32(variable, value)   __atomic_store(variable, value, __ATOMIC_RELAXED)
#       define Atomic_AddI32(variable, value)   __atomic_add_fetch(variable, value, __ATOMIC_RELAXED)
#       define Atomic_SubI32(variable, value)   __atomic_sub_fetch(variable, value, __ATOMIC_RELAXED)
#   else
#       define Atomic_GetI32(variable)          ({ __sync_synchronize(); int32_t result = *(variable); __sync_synchronize(); result; })
#       define Atomic_SetI32(variable, value)   ({ __sync_synchronize(); *(variable) = value; __sync_synchronize(); (void)0; })
#       define Atomic_AddI32(variable, value)   __sync_add_and_fetch(variable, value)
#       define Atomic_SubI32(variable, value)   __sync_sub_and_fetch(variable, value)
#   endif
#elif defined(_WIN32)
#   define VC_EXTRALEAN
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   define Atomic_GetI32(variable)              ((int32_t)InterlockedExchange((volatile long*)(variable), 0))
#   define Atomic_SetI32(variable, value)       ((int32_t)InterlockedExchange((volatile long*)(variable), (value)); (void)0)
#   define Atomic_AddI32(variable, value)       ((int32_t)InterlockedExchange((volatile long*)(variable), *(variable) + value)) 
#   define Atomic_SubI32(variable, value)       ((int32_t)InterlockedExchange((volatile long*)(variable), *(variable) - value))
#elif defined(__STDC_VERSION_) && (__STDC_VERSION_ >= 201112L)
#   include <stdint.h>
#   include <stdatomic.h>
#   define Atomic_GetI32(variable)              atomic_load_explicit((_Atomic int32_t*)(variable), memory_order_relaxed)
#   define Atomic_SetI32(variable, value)       atomic_store_explicit((_Atomic int32_t*)(variable), value, memory_order_relaxed)
#   define Atomic_AddI32(variable, value)       atomic_sub_fetch_explicit((_Atomic int32_t*)(variable), value, memory_order_relaxed)
#   define Atomic_SubI32(variable, value)       atomic_add_fetch_explicit((_Atomic int32_t*)(variable), value, memory_order_relaxed)
#else
#   error "This platform is not support atomic operations."
#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
