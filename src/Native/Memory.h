#pragma once

#include <stdint.h>

// --------------------------------------
// Main functions
// --------------------------------------

#if !defined(NDEBUG)
#define MemoryAlloc(size, align)            MemoryAllocDebug(size, align, __FUNCTION__, __FILE__, __LINE__)
#define MemoryRealloc(ptr, size, align)     MemoryReallocDebug(ptr, size, align, __FUNCTION__, __FILE__, __LINE__)
#define MemoryFree(ptr)                     MemoryFreeDebug(ptr, __FUNCTION__, __FILE__, __LINE__)

void*   MemoryAllocDebug(int32_t size, int32_t align, const char* func, const char* file, int32_t line);
void*   MemoryReallocDebug(void* ptr, int32_t size, int32_t align, const char* func, const char* file, int32_t line);
void    MemoryFreeDebug(void* ptr, const char* func, const char* file, int32_t line);
#else
void*   MemoryAlloc(int32_t size, int32_t align);
void*   MemoryRealloc(void* ptr, int32_t size, int32_t align);
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

struct MemoryTracker
{
    int32_t     markAllocations;

                MemoryTracker();
                ~MemoryTracker();
};

