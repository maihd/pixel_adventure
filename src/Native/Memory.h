#pragma once

#include <stdint.h>

// --------------------------------------
// Memory allocations
// --------------------------------------

/// Allocate memory with given size and align
#define MemoryAlloc(size, align)                MemoryAllocImpl(size, align)

/// Resize and realign a memory block
/// Allocate new if required
#define MemoryRealloc(ptr, size, align)         MemoryReallocImpl(ptr, size, align)

/// Free memory block
#define MemoryFree(ptr)                         MemoryFreeImpl(ptr)

#if !defined(NDEBUG)
#define MemoryAllocImpl(size, align)            MemoryAllocDebug(size, align, __FUNCTION__, __FILE__, __LINE__)
#define MemoryReallocImpl(ptr, size, align)     MemoryReallocDebug(ptr, size, align, __FUNCTION__, __FILE__, __LINE__)
#define MemoryFreeImpl(ptr)                     MemoryFreeDebug(ptr, __FUNCTION__, __FILE__, __LINE__)

void*   MemoryAllocDebug(int32_t size, int32_t align, const char* func, const char* file, int32_t line);
void*   MemoryReallocDebug(void* ptr, int32_t size, int32_t align, const char* func, const char* file, int32_t line);
void    MemoryFreeDebug(void* ptr, const char* func, const char* file, int32_t line);
#else
#define MemoryAllocImpl(size, align)            MemoryAllocNDebug(size, align)
#define MemoryReallocImpl(ptr, size, align)     MemoryReallocNDebug(ptr, size, align)
#define MemoryFreeImpl(ptr)                     MemoryFreeNDebug(ptr)

void*   MemoryAllocNDebug(int32_t size, int32_t align);
void*   MemoryReallocNDebug(void* ptr, int32_t size, int32_t align);
void    MemoryFreeNDebug(void* ptr);
#endif

// --------------------------------------
// Memory manipulating
// --------------------------------------

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

#ifndef DISABLE_MEMORY_TRACKING
#define MEMORY_TRACKING() MemoryTracker __MEMORY_TRACKER_##__LINE__##__
#else
#define MEMORY_TRACKING() do {} while(0)
#endif

struct MemoryTracker
{
    int32_t     markAllocations;

                MemoryTracker();
                ~MemoryTracker();
};

