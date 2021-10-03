#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------
// Main functions
// --------------------------------------

#if !defined(NDEBUG)
#define MemoryAlloc(size)           MemoryAllocDebug(size, __FUNCTION__, __FILE__, __LINE__)
#define MemoryRealloc(ptr, size)    MemoryReallocDebug(ptr, size, __FUNCTION__, __FILE__, __LINE__)
#define MemoryFree(ptr)             MemoryFreeDebug(ptr, __FUNCTION__, __FILE__, __LINE__)

void*   MemoryAllocDebug(int32_t size, const char* func, const char* file, int32_t line);
void*   MemoryReallocDebug(void* ptr, int32_t size, const char* func, const char* file, int32_t line);
void    MemoryFreeDebug(void* ptr, const char* func, const char* file, int32_t line);
#else
void*   MemoryAlloc(int32_t size);
void*   MemoryRealloc(void* ptr, int32_t size);
void    MemoryFree(void* ptr);
#endif

void*   MemoryInit(void* ptr, const int32_t value, const int32_t size);
void*   MemoryCopy(void* dst, const void* src, const int32_t size);
void*   MemoryMove(void* dst, const void* src, const int32_t size);

// --------------------------------------
// Report memory
// --------------------------------------

void MemoryDumpAllocs(void);

// ------------------------------------
// Memory system information functions
// ------------------------------------

int MemoryPageSize(void);

// --------------------------------------
// Utils
// --------------------------------------

#ifdef __cplusplus
struct MemoryTracker
{
    int32_t     markAllocations;

                MemoryTracker();
                ~MemoryTracker();
};
#endif

// End of extern C
#ifdef __cplusplus
}
#endif
