#include <malloc/_malloc.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Memory.h"
#include "HeapLayers.h"
#include "Misc/Logging.h"

#ifndef _WIN32
#define _aligned_malloc     aligned_alloc
#define _aligned_realloc(ptr, alignment, size)    ({ typeof(ptr) ptr_x; posix_memalign(&ptr_x, alignment, size); ptr; })
#define _aligned_free(ptr)  (posix_memalign(&(ptr), 0, 0))
#endif

#if !defined(NDEBUG)

// ----------------------
// Internal types
// ----------------------

struct AllocDesc
{
    void*               ptr;
    int32_t             size;
    int32_t             align;

    const char*         tag;
    const char*         func;
    const char*         file;
    int32_t             line;

    time_t              createTime;
    time_t              modifiedTime;

    int32_t             modifiedCount;
    int32_t             addressChangedCount;

    struct AllocDesc*   next;
};

// ----------------------------
// Tracking memory helpers
// ----------------------------

constexpr int32_t ALLOC_DESC_COUNT = 64;
static struct
{
    PagedFreeList   freeAllocDescs;
    AllocDesc*      hashAllocDescs[ALLOC_DESC_COUNT];

    int32_t         allocSize = 0;
    int32_t         allocations = 0;
    int32_t         allocCalled = 0;
    int32_t         reallocCalled = 0;
    int32_t         freeCalled = 0;
} gAllocStore;

static void AddAlloc(void* ptr, int32_t size, int32_t align, const char* tag, const char* func, const char* file, int32_t line)
{
    AllocDesc* allocDesc = (AllocDesc*)gAllocStore.freeAllocDescs.Alloc(sizeof(AllocDesc));

    allocDesc->ptr      = ptr;
    allocDesc->size     = size;
    allocDesc->align    = align;

    allocDesc->tag      = tag;
    allocDesc->func     = func;
    allocDesc->file     = file;
    allocDesc->line     = line;

    allocDesc->createTime = time(nullptr);
    allocDesc->modifiedTime = allocDesc->createTime;
    
    allocDesc->modifiedCount = 0;
    allocDesc->addressChangedCount = 0;

    uint64_t ptrHash = ((uint64_t)ptr) & (ALLOC_DESC_COUNT - 1);
    allocDesc->next = gAllocStore.hashAllocDescs[ptrHash];
    gAllocStore.hashAllocDescs[ptrHash] = allocDesc;

    gAllocStore.allocSize += size;
    gAllocStore.allocations++;
}

static void UpdateAlloc(void* ptr, void* newPtr, int32_t size, int32_t align, const char* tag, const char* func, const char* file, int32_t line)
{
    uint64_t ptrHash = ((uint64_t)ptr) & (ALLOC_DESC_COUNT - 1);

    AllocDesc* prevAllocDesc = nullptr;
    AllocDesc* allocDesc = gAllocStore.hashAllocDescs[ptrHash];
    while (allocDesc != nullptr && allocDesc->ptr != ptr)
    {
        prevAllocDesc = allocDesc;
        allocDesc = allocDesc->next;
    }

    assert(allocDesc != nullptr && "This block is not allocated by our system, please check your memory source!");
    assert(strcmp(allocDesc->tag, tag) != 0 && "You attempt free memory with difference tag when allocated!");

    allocDesc->ptr = newPtr;
    allocDesc->size = size;
    allocDesc->func = func;
    allocDesc->file = file;
    allocDesc->line = line;

    allocDesc->modifiedTime = time(nullptr);
    allocDesc->modifiedCount++;
    allocDesc->addressChangedCount += (ptr != newPtr);

    gAllocStore.allocSize -= allocDesc->size;
    gAllocStore.allocSize += size;

    uint64_t newPtrHash = ((uint64_t)newPtr) & (ALLOC_DESC_COUNT - 1);
    if (newPtrHash != ptrHash)
    {
        if (prevAllocDesc)
        {
            prevAllocDesc->next = allocDesc->next;
        }
        else
        {
            gAllocStore.hashAllocDescs[ptrHash] = allocDesc->next;
        }

        allocDesc->next = gAllocStore.hashAllocDescs[newPtrHash];
        gAllocStore.hashAllocDescs[newPtrHash] = allocDesc;
    }
}

static void RemoveAlloc(void* ptr, const char* tag, const char* func, const char* file, int32_t line)
{
    uint64_t ptrHash = ((uint64_t)ptr) & (ALLOC_DESC_COUNT - 1);

    AllocDesc* prevAllocDesc = nullptr;
    AllocDesc* allocDesc = gAllocStore.hashAllocDescs[ptrHash];
    while (allocDesc != nullptr && allocDesc->ptr != ptr)
    {
        prevAllocDesc = allocDesc;
        allocDesc = allocDesc->next;
    }

    assert(allocDesc != nullptr && "This block is not allocated by our system! Are you attempt to double-free?");
    assert(strcmp(allocDesc->tag, tag) != 0 && "You attempt free memory with difference tag when allocated!");

    gAllocStore.freeAllocDescs.Free(allocDesc);

    if (prevAllocDesc)
    {
        prevAllocDesc->next = allocDesc->next;
    }
    else
    {
        gAllocStore.hashAllocDescs[ptrHash] = allocDesc->next;
    }

    gAllocStore.allocSize -= allocDesc->size;
    gAllocStore.allocations--;
}

void* Memory_AllocDebug(const char* tag, int32_t size, int32_t align, const char* func, const char* file, int32_t line)
{
    assert(size > 0 && "Request size must be greater than 0.");

    gAllocStore.allocCalled++;

    void* ptr = _aligned_malloc((size_t)size, (size_t)align);
    AddAlloc(ptr, size, align, tag, func, file, line);
    return ptr;
}

void* Memory_ReallocDebug(const char* tag, void* ptr, int32_t size, int32_t align, const char* func, const char* file, int32_t line)
{
    assert(size > 0 && "Request size must be greater than 0.");

    gAllocStore.reallocCalled++;

    void* newPtr = _aligned_realloc(ptr, (size_t)size, (size_t)align);
    if (ptr == nullptr)
    {
        AddAlloc(newPtr, size, align, tag, func, file, line);
    }
    else
    {
        UpdateAlloc(ptr, newPtr, size, align, tag, func, file, line);
    }
    return newPtr;
}

void Memory_FreeDebug(const char* tag, void* ptr, const char* func, const char* file, int32_t line)
{
    //DebugAssert(ptr != nullptr, "Attempt free nullptr at %s:%d:%s", func, file, line);
    gAllocStore.freeCalled++;

    if (ptr)
    {
        RemoveAlloc(ptr, func, tag, file, line);
        _aligned_free(ptr);
    }
}

void Memory_DumpAllocs(void)
{
    assert(gAllocStore.allocations >= 0 && "Internal system error!");

    if (gAllocStore.allocations == 0)
    {
        Log_Info("Memory", "No memory allocations\n");
        return;
    }

    Log_Info("Memory", "Address\t\tSize\t\tModified\tSource\n");
    for (int32_t i = 0; i < ALLOC_DESC_COUNT; i++)
    {
        AllocDesc* allocDesc = gAllocStore.hashAllocDescs[i];
        while (allocDesc != nullptr)
        {
            Log_Info("Memory", "0x%p\t%d\t\t%d\t\t%s:%d:%s\n",
                allocDesc->ptr, 
                allocDesc->size, 
                allocDesc->modifiedCount, 
                allocDesc->file, allocDesc->line, allocDesc->func
            );

            allocDesc = allocDesc->next;
        }
    }
}

MemoryTracker::MemoryTracker()
    : markAllocations(gAllocStore.allocations)
{
}

MemoryTracker::~MemoryTracker()
{
    assert(markAllocations <= gAllocStore.allocations && "Memory leaks occurred!");
}
// END OF #if !defined(NDEBUG)
#else
void* Memory_AllocNDebug(int32_t size, int32_t align)
{
    return _aligned_malloc((size_t)size, (size_t)align);
}

void* Memory_ReallocNDebug(void* ptr, int32_t size, int32_t align)
{
    return _aligned_realloc(ptr, (size_t)size, (size_t)align);
}

void Memory_FreeNDebug(void* ptr)
{
    _aligned_free(ptr);
}

void MemoryDumpAllocs(void)
{
}

MemoryTracker::MemoryTracker()
    : markAllocations(0)
{
}

MemoryTracker::~MemoryTracker()
{
}
#endif

void* Memory_Init(void* ptr, const int32_t value, const int32_t size)
{
    return memset(ptr, value, (size_t)size);
}

void* Memory_Copy(void* dst, const void* src, const int32_t size)
{
    return memcpy(dst, src, (size_t)size);
}

void* Memory_Move(void* dst, const void* src, const int32_t size)
{
    return memmove(dst, src, size);
}

// ------------------------------------
// Memory system information functions
// ------------------------------------

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int32_t Memory_PageSize(void)
{
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    return (int32_t)systemInfo.dwPageSize;
}

#elif defined(__unix__)
#include <unistd.h>
int32_t MemoryPageSize(void)
{
    return getpagesize();
}
#else
int32_t MemoryPageSize(void)
{
    return 4096;
}
#endif

#include <imgui/imgui.h>

// Open an debug window to view your memory allocations
void ImGui::DumpMemoryAllocs(ImGuiDumpMemoryFlags flags)
{
    bool render = true;

    bool openWindow = (flags & ImGuiDumpMemoryFlags_OpenWindow) != 0;
    if (openWindow)
    {
        render = ImGui::Begin("Memory Allocations", nullptr, ImGuiWindowFlags_NoFocusOnAppearing);
    }

    #if BUILD_PROFILING
    if (render)
    {
        ImGui::Text("AllocSize: %.2lfKB", gAllocStore.allocSize / 1024.0);
        ImGui::Text("Allocations: %d", gAllocStore.allocations);
        ImGui::Text("AllocCalled: %d", gAllocStore.allocCalled);
        ImGui::Text("ReallocCalled: %d", gAllocStore.reallocCalled);
        ImGui::Text("FreeCalled: %d", gAllocStore.freeCalled);

        ImGui::Columns(8);
        ImGui::SetColumnWidth(0, 120);
        ImGui::SetColumnWidth(1, 144);
        ImGui::SetColumnWidth(2, 88);
        ImGui::SetColumnWidth(3, 88);
        ImGui::SetColumnWidth(4, 120);
        ImGui::SetColumnWidth(5, 146);
        ImGui::SetColumnWidth(6, 180);

        ImGui::Text("Tag");
        ImGui::NextColumn();
        ImGui::Text("Address");
        ImGui::NextColumn();
        ImGui::Text("Size");
        ImGui::NextColumn();
        ImGui::Text("Align");
        ImGui::NextColumn();
        ImGui::Text("ModifiedCount");
        ImGui::NextColumn();
        ImGui::Text("AddressChangedCount");
        ImGui::NextColumn();
        ImGui::Text("Function");
        ImGui::NextColumn();
        ImGui::Text("Source");
        ImGui::NextColumn();

        ImGui::Columns(1);
        if (ImGui::BeginChild("Allocations List"))
        {
            ImGui::Columns(8);
            ImGui::SetColumnWidth(0, 112);
            ImGui::SetColumnWidth(1, 144);
            ImGui::SetColumnWidth(2, 88);
            ImGui::SetColumnWidth(3, 88);
            ImGui::SetColumnWidth(4, 120);
            ImGui::SetColumnWidth(5, 146);
            ImGui::SetColumnWidth(6, 180);

            for (int i = 0; i < ALLOC_DESC_COUNT; i++)
            {
                AllocDesc* allocDesc = gAllocStore.hashAllocDescs[i];
                while (allocDesc != nullptr)
                {
                    ImGui::Text("%s", allocDesc->tag);
                    ImGui::NextColumn();

                    ImGui::Text("0x%p", allocDesc->ptr);
                    ImGui::NextColumn();
                    ImGui::Text("%dB", allocDesc->size);
                    ImGui::NextColumn();
                    ImGui::Text("%d", allocDesc->align);
                    ImGui::NextColumn();

                    ImGui::Text("%d", allocDesc->modifiedCount);
                    ImGui::NextColumn();
                    ImGui::Text("%d", allocDesc->addressChangedCount);
                    ImGui::NextColumn();

                    ImGui::Text("%s", allocDesc->func);
                    ImGui::NextColumn();

                    ImGui::BulletText("%s:%d", allocDesc->file, allocDesc->line, allocDesc->func);
                    ImGui::NextColumn();

                    // Implement goto file
                    //if (ImGui::IsItemHovered())
                    //{
                    //    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    //}
                    //if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    //{
                    //    char command[1024];
                    //    sprintf(command, "code --goto %s:%d", allocDesc->file, allocDesc->line);
                    //    system(command);
                    //
                    //    ImGui::SetNextFrameWantCaptureMouse(false);
                    //}

                    allocDesc = allocDesc->next;
                }
            }
        }

        ImGui::EndChild();
    }
    #else
    ImGui::Text("Profiling is not enabled!");
    #endif

    if (openWindow)
    {
        ImGui::End();
    }
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
