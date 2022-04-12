#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Memory.h"
#include "HeapLayers.h"

#if !defined(NDEBUG)

// ----------------------
// Internal types
// ----------------------

struct AllocDesc
{
    void*               ptr;
    int32_t             size;

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

static void AddAlloc(void* ptr, int32_t size, const char* func, const char* file, int32_t line)
{
    AllocDesc* allocDesc = (AllocDesc*)gAllocStore.freeAllocDescs.Alloc(sizeof(AllocDesc));

    allocDesc->ptr  = ptr;
    allocDesc->size = size;
    allocDesc->func = func;
    allocDesc->file = file;
    allocDesc->line = line;

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

static void UpdateAlloc(void* ptr, void* newPtr, int32_t size, const char* func, const char* file, int32_t line)
{
    uint64_t ptrHash = ((uint64_t)ptr) & (ALLOC_DESC_COUNT - 1);

    AllocDesc* prevAllocDesc = nullptr;
    AllocDesc* allocDesc = gAllocStore.hashAllocDescs[ptrHash];
    while (allocDesc != nullptr && allocDesc->ptr != ptr)
    {
        prevAllocDesc = allocDesc;
        allocDesc = allocDesc->next;
    }

    assert(allocDesc != nullptr, "This block is not allocated by our system, please check your memory source!");
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

static void RemoveAlloc(void* ptr, const char* func, const char* file, int32_t line)
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

void* MemoryAllocDebug(int32_t size, int32_t align, const char* func, const char* file, int32_t line)
{
    assert(size > 0 && "Request size must be greater than 0.");

    gAllocStore.allocCalled++;

    void* ptr = _aligned_malloc((size_t)size, (size_t)align);
    AddAlloc(ptr, size, func, file, line);
    return ptr;
}

void* MemoryReallocDebug(void* ptr, int32_t size, int32_t align, const char* func, const char* file, int32_t line)
{
    assert(size > 0 && "Request size must be greater than 0.");

    gAllocStore.reallocCalled++;

    void* newPtr = _aligned_realloc(ptr, (size_t)size, (size_t)align);
    if (ptr == nullptr)
    {
        AddAlloc(newPtr, size, func, file, line);
    }
    else
    {
        UpdateAlloc(ptr, newPtr, size, func, file, line);
    }
    return newPtr;
}

void MemoryFreeDebug(void* ptr, const char* func, const char* file, int32_t line)
{
    //DebugAssert(ptr != nullptr, "Attempt free nullptr at %s:%d:%s", func, file, line);
    gAllocStore.freeCalled++;

    if (ptr)
    {
        RemoveAlloc(ptr, func, file, line);
        _aligned_free(ptr);
    }
}

void MemoryDumpAllocs(void)
{
    assert(gAllocStore.allocations >= 0 && "Internal system error!");

    if (gAllocStore.allocations == 0)
    {
        printf("No memory allocations\n");
        return;
    }

    printf("Address\t\tSize\t\tModified\tSource\n");
    for (int32_t i = 0; i < ALLOC_DESC_COUNT; i++)
    {
        AllocDesc* allocDesc = gAllocStore.hashAllocDescs[i];
        while (allocDesc != nullptr)
        {
            printf("0x%p\t%d\t\t%d\t\t%s:%d:%s\n", 
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
void* MemoryAllocNDebug(int32_t size, int32_t align)
{
    return _aligned_malloc((size_t)size, (size_t)align);
}

void* MemoryReallocNDebug(void* ptr, int32_t size, int32_t align)
{
    return _aligned_realloc(ptr, (size_t)size, (size_t)align);
}

void MemoryFreeNDebug(void* ptr)
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

void* MemoryInit(void* ptr, const int32_t value, const int32_t size)
{
    return memset(ptr, value, (size_t)size);
}

void* MemoryCopy(void* dst, const void* src, const int32_t size)
{
    return memcpy(dst, src, (size_t)size);
}

void* MemoryMove(void* dst, const void* src, const int32_t size)
{
    return memmove(dst, src, size);
}

// ------------------------------------
// Memory system information functions
// ------------------------------------

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int32_t MemoryPageSize(void)
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
