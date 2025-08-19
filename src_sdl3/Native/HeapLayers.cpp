#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "HeapLayers.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <sys/mman.h>
#else
#error "The current system doesnot support paged allocations"
#endif

void* PagedHeap::Alloc(int32_t size)
{
#if defined(_WIN32)
    return VirtualAlloc(nullptr, (SIZE_T)size, MEM_COMMIT, PAGE_READWRITE);
#elif defined(__unix__)
    return mmap(nullptr, (size_t)allocSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0);
#endif
}

void* PagedHeap::Realloc(void* ptr, int32_t size)
{
#if defined(_WIN32)
    return VirtualAlloc(ptr, (SIZE_T)size, MEM_COMMIT, PAGE_READWRITE);
#elif defined(__unix__)
    return mmap(ptr, (size_t)allocSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0);
#endif
}

void PagedHeap::Free(void* ptr)
{
#if defined(_WIN32)
    VirtualFree(ptr, 0, MEM_DECOMMIT);
#elif defined(__unix__)
    munmap(ptr, 0);
#endif
}

int32_t PagedHeap::GetSize(void* ptr)
{
    assert(false && "NOT IMPLEMENTED");
    return 0;
}

void* PagedFreeList::Alloc(int32_t size)
{
    if (!freeItem)
    {
        const int32_t itemSize = size;
        const int32_t allocSize = 64 * 1024;
        const int32_t itemsPerBatch = (allocSize - sizeof(Page)) / itemSize;

    #if defined(_WIN32)
        Page* page = (Page*)VirtualAlloc(nullptr, (SIZE_T)allocSize, MEM_COMMIT, PAGE_READWRITE);
    #elif defined(__unix__) || defined(__APPLE__)
        Page* page = (Page*)mmap(nullptr, (size_t)allocSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0);
    #else
        #error "The current system doesnot support paged allocations"
    #endif

        page->Next = allocedPages;
        page->itemSize = size;
        allocedPages = page;

        void* allocBatch = page + 1;
        for (int32_t i = 0; i < itemsPerBatch; i++)
        {
            Free((uint8_t*)allocBatch + i * itemSize);
        }
    }

    Item* item = freeItem;
    freeItem = item->next;
    return item;
}

void PagedFreeList::Free(void* ptr)
{
    Item* item = (Item*)ptr;
    item->next = freeItem;
    freeItem = item;
}

int32_t PagedFreeList::GetSize(void* ptr) const
{
    assert(false && "NOT IMPLEMENTED");
    return 0;
}

PagedFreeList::~PagedFreeList()
{
    Page* page = allocedPages;
    while (page != nullptr)
    {
        Page* next = page->Next;
    #if defined(_WIN32)
        //VirtualFree(page, 0, MEM_DECOMMIT);
    #else

    #endif
        page = next;
    }

    freeItem = nullptr;
    allocedPages = nullptr;
}
