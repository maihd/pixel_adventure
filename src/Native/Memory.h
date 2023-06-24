#pragma once

#include <stdint.h>
#include "Misc/Compiler.h"

// --------------------------------------
// Memory constructor & destructor
// --------------------------------------

struct MemoryNewWrapper {};
__forceinline void* operator new(size_t, MemoryNewWrapper, void* ptr) { return ptr; }
__forceinline void  operator delete(void*, MemoryNewWrapper, void*)   {             } // This is only required so we can use the symmetrical new()

#define MemoryNewPlacement(ptr)                 new (MemoryNewWrapper(), ptr)
#define MemoryNew(Type)                         new (MemoryNewWrapper(), MemoryAllocImpl(sizeof(Type), alignof(Type)))
#define MemoryDelete(ptr)                       MemoryDeleteImpl(ptr)

template <typename T>
__forceinline void MemoryDeleteImpl(T* ptr)
{ 
    if (ptr)
    { 
        ptr->~T();
        MemoryFree(ptr);
    }
}

// --------------------------------------
// Memory allocations API
// --------------------------------------

/// Allocate memory with given size and align, with given specify tag for tracking
#define MemoryAllocTag(tag, size, align)                MemoryAllocImpl(tag, size, align)

/// Resize and realign a memory block, with given specify tag for tracking
/// Allocate new if required
#define MemoryReallocTag(tag, ptr, size, align)         MemoryReallocImpl(tag, ptr, size, align)

/// Free memory block, with given specify tag for tracking
#define MemoryFreeTag(tag, ptr)                         MemoryFreeImpl(tag, ptr)

/// Allocate memory with given size and align
#define MemoryAlloc(size, align)                        MemoryAllocImpl("Common", size, align)

/// Resize and realign a memory block
/// Allocate new if required
#define MemoryRealloc(ptr, size, align)                 MemoryReallocImpl("Common", ptr, size, align)

/// Free memory block   
#define MemoryFree(ptr)                                 MemoryFreeImpl("Common", ptr)

#if !defined(NDEBUG)
#define MemoryAllocImpl(tag, size, align)               MemoryAllocDebug(tag, size, align, __FUNCTION__, __FILE__, __LINE__)
#define MemoryReallocImpl(tag, ptr, size, align)        MemoryReallocDebug(tag, ptr, size, align, __FUNCTION__, __FILE__, __LINE__)
#define MemoryFreeImpl(tag, ptr)                        MemoryFreeDebug(tag, ptr, __FUNCTION__, __FILE__, __LINE__)
#else
#define MemoryAllocImpl(tag, size, align)               MemoryAllocNDebug(size, align)
#define MemoryReallocImpl(tag, ptr, size, align)        MemoryReallocNDebug(ptr, size, align)
#define MemoryFreeImpl(tag, ptr)                        MemoryFreeNDebug(ptr)
#endif

// --------------------------------------
// Memory allocations implementations
// --------------------------------------

#if !defined(NDEBUG)
void*   MemoryAllocDebug(const char* tag, int32_t size, int32_t align, const char* func, const char* file, int32_t line);
void*   MemoryReallocDebug(const char* tag, void* ptr, int32_t size, int32_t align, const char* func, const char* file, int32_t line);
void    MemoryFreeDebug(const char* tag, void* ptr, const char* func, const char* file, int32_t line);
#else
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

enum ImGuiDumpMemoryFlags
{
    ImGuiDumpMemoryFlags_None,
    ImGuiDumpMemoryFlags_OpenWindow = 1 << 0,

    // Default
    ImGuiDumpMemoryFlags_Default = ImGuiDumpMemoryFlags_OpenWindow,
};

namespace ImGui
{
    // Open an debug window to view your memory allocations
    void DumpMemoryAllocs(ImGuiDumpMemoryFlags flags = ImGuiDumpMemoryFlags_Default);
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
