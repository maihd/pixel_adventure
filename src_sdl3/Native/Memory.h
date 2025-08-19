#pragma once

#include <stdint.h>
#include "Misc/Compiler.h"

// @todo: Add Arena
// @todo: Refactor to MaiCStyle

// --------------------------------------
// Memory constructor & destructor
// --------------------------------------

#ifdef __cplusplus

struct MemoryNewWrapper {};
__forceinline void* operator new(size_t, MemoryNewWrapper, void* ptr) { return ptr; }
__forceinline void  operator delete(void*, MemoryNewWrapper, void*)   {             } // This is only required so we can use the symmetrical new()

#define Memory_NewPlacement(ptr)                (new (MemoryNewWrapper(), ptr))
#define Memory_New(Type)                        (new (MemoryNewWrapper(), Memory_AllocTag("C++", sizeof(Type), alignof(Type))))
#define Memory_Delete(ptr)                      (Memory_CallDestructor(ptr), Memory_FreeTag("C++", ptr))

template <typename T>
__forceinline void Memory_CallDestructor(T* ptr)
{ 
    if (ptr)
    { 
        ptr->~T();
    }
}

#endif

// --------------------------------------
// Memory allocations API
// --------------------------------------

/// Allocate memory with given size and align, with given specify tag for tracking
#define Memory_AllocTag(tag, size, align)               Memory_AllocImpl(tag, size, align)

/// Resize and realign a memory block, with given specify tag for tracking
/// Allocate new if required
#define Memory_ReallocTag(tag, ptr, size, align)        Memory_ReallocImpl(tag, ptr, size, align)

/// Free memory block, with given specify tag for tracking
#define Memory_FreeTag(tag, ptr)                        Memory_FreeImpl(tag, ptr)

/// Allocate memory with given size and align
#define Memory_Alloc(size, align)                       Memory_AllocImpl("Common", size, align)

/// Resize and realign a memory block
/// Allocate new if required
#define Memory_Realloc(ptr, size, align)                Memory_ReallocImpl("Common", ptr, size, align)

/// Free memory block   
#define Memory_Free(ptr)                                Memory_FreeImpl("Common", ptr)

#if !defined(NDEBUG)
#define Memory_AllocImpl(tag, size, align)              Memory_AllocDebug(tag, size, align, __FUNCTION__, __FILE__, __LINE__)
#define Memory_ReallocImpl(tag, ptr, size, align)       Memory_ReallocDebug(tag, ptr, size, align, __FUNCTION__, __FILE__, __LINE__)
#define Memory_FreeImpl(tag, ptr)                       Memory_FreeDebug(tag, ptr, __FUNCTION__, __FILE__, __LINE__)
#else
#define Memory_AllocImpl(tag, size, align)              Memory_AllocNDebug(size, align)
#define Memory_ReallocImpl(tag, ptr, size, align)       Memory_ReallocNDebug(ptr, size, align)
#define Memory_FreeImpl(tag, ptr)                       Memory_FreeNDebug(ptr)
#endif

// --------------------------------------
// Memory allocations implementations
// --------------------------------------

#if !defined(NDEBUG)
void*   Memory_AllocDebug(const char* tag, int32_t size, int32_t align, const char* func, const char* file, int32_t line);
void*   Memory_ReallocDebug(const char* tag, void* ptr, int32_t size, int32_t align, const char* func, const char* file, int32_t line);
void    Memory_FreeDebug(const char* tag, void* ptr, const char* func, const char* file, int32_t line);
#else
void*   Memory_AllocNDebug(int32_t size, int32_t align);
void*   Memory_ReallocNDebug(void* ptr, int32_t size, int32_t align);
void    Memory_FreeNDebug(void* ptr);
#endif

// --------------------------------------
// Memory manipulating
// --------------------------------------

void*   Memory_Init(void* ptr, const int32_t value, const int32_t size);
void*   Memory_Copy(void* dst, const void* src, const int32_t size);
void*   Memory_Move(void* dst, const void* src, const int32_t size);

// --------------------------------------
// Report memory
// --------------------------------------

void    Memory_DumpAllocs(void);

// ------------------------------------
// Memory system information functions
// ------------------------------------

int32_t Memory_PageSize(void);

// --------------------------------------
// Utils
// --------------------------------------

#ifdef __cplusplus

#ifndef DISABLE_MEMORY_TRACKING
#define MEMORY_TRACKING() MemoryTracker __MEMORY_TRACKER_##__LINE__##__
#else
#define MEMORY_TRACKING() do {} while(0)
#endif

// @todo: Convert to C version
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

#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
