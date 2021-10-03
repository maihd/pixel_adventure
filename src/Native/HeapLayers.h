#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

template <typename SuperHeap>
struct SizeHeap : public SuperHeap
{
    inline void* Alloc(int32_t size)
    {
        int32_t* item = (int32_t*)SuperHeap::Alloc(sizeof(int32_t) + size);
        if (item)
        {
            *item = size;
        }
        return item + 1;
    }

    inline void* Realloc(void* ptr, int32_t size)
    {
        int32_t* newItem = (int32_t*)SuperHeap::Realloc(ptr ? ((int32_t*)ptr - 1) : nullptr, sizeof(int32_t));
        if (newItem)
        {
            *newItem = size;
        }
        return newItem + 1;
    }

    inline void Free(void* ptr)
    {
        SuperHeap::Free((int32_t*)ptr - 1);
    }

    inline int32_t GetSize(void* ptr)
    {
        return *((int32_t*)ptr - 1);
    }
};

struct StrictSegHeapTraits
{
    // Return the bin that this size falls into
    static inline int32_t GetSizeBin(int32_t size)
    {
        int32_t bin = 0;
        size--;
        while (size > 7)
        {
            size = size >> 1;
            bin++;
        }
        return bin;
    }
 
    // Return the size of allocations in this bin
    static inline int32_t GetBinMaxSize(int32_t bin)
    {
        return 1 << (bin + 3);
    }
};

template <int32_t BinCount, typename Traits, typename LittleHeap, typename SuperHeap>
struct StrictSegHeap : public SuperHeap
{
    // The array of little heaps (bins), one for each allocation size
    LittleHeap LittleHeaps[BinCount];

    // Return the bin that this allocation falls into
    inline int32_t InnerGetSizeBin(int32_t size)
    {
        if (size > Traits::GetBinMaxSize(BinCount - 1))
        {
            return BinCount;
        }

        return Traits::GetSizeBin(size);
    }

    inline void* Alloc(int32_t size)
    {
        int32_t sizeBin = InnerGetSizeBin(size);
        if (sizeBin >= BinCount)
        {
            return SuperHeap::Alloc(size);
        }

        int32_t allocSize = Traits::GetBinMaxSize(sizeBin);
        return LittleHeaps[sizeBin].Alloc(allocSize);
    }

    inline void* Realloc(void* ptr, int32_t size)
    {
        if (ptr)
        {
            int32_t allocedSize = SuperHeap::GetSize(ptr);
            int32_t allocedSizeBin = InnerGetSizeBin(allocedSize);
            if (allocedSizeBin >= BinCount)
            {
                return SuperHeap::Realloc(ptr, size);
            }

            // Loose reallocation: only realloc if bigger or at least twice smaller
            if ((size > allocedSize) || (size < (allocedSize >> 1)))
            {
                void* newPtr = Alloc(size);
                if (newPtr)
                {
                    int32_t copySize = allocedSize > size ? size : allocedSize;
                    memcpy(newPtr, ptr, copySize);
                }
                LittleHeaps[allocedSizeBin].Free(ptr);

                return newPtr;
            }

            return ptr;
        }

        return Alloc(size);
    }

    inline void Free(void* ptr)
    {
        if (ptr)
        {
            int32_t size = SuperHeap::GetSize(ptr);
            int32_t sizeBin = InnerGetSizeBin(size);
            if (sizeBin >= BinCount)
            {
                SuperHeap::Free(ptr);
            }
            else
            {
                LittleHeaps[sizeBin].Free(ptr);
            }
        }
    }
};

struct PagedHeap
{
    void*       Alloc(int32_t size);
    void*       Realloc(void* ptr, int32_t size);
    void        Free(void* ptr);
    int32_t     GetSize(void* ptr);
};

struct PagedFreeList
{
    struct Item
    {
        Item*       next;
    };

    struct Page
    {
        int32_t     pageSize;
        int32_t     itemSize;

        Page*       Next;
    };

    Item*           freeItem;
    Page*           allocedPages;

    void*           Alloc(int32_t size);
    void            Free(void* ptr);
    int32_t         GetSize(void* ptr) const;

                    ~PagedFreeList();

    inline PagedFreeList()
        : freeItem(nullptr)
        , allocedPages(nullptr)
    {
    }
};

struct CrtMalloc
{
    inline void* Alloc(int32_t size)
    {
        return malloc((size_t)size);
    }

    inline void* Realloc(void* ptr, int32_t size)
    {
        return realloc(ptr, (size_t)size);
    }

    inline void Free(void* ptr)
    {
        free(ptr);
    }
};
