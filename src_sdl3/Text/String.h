#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include "Misc/Compiler.h"

constexpr uint32_t STRING_MEMTAG_HEAP = 0xac8b19d1U;
constexpr uint32_t STRING_MEMTAG_WEAK = 0x2e9497f4U;

/// StringBuffer
/// @note: make sure StringBuffer is 16bytes aligned
#pragma warning(disable : 4200)
typedef struct StringBuffer
{
    uint32_t    memtag;
    int32_t     memref;

    int32_t     length;
    int32_t     capacity;

    char        data[];
} StringBuffer;

#ifdef __cplusplus
extern "C" {
#endif

const char*             String_Ref(const char* source);
const char*             String_New(const char* source);
void                    String_Free(const char* target);

const char*             String_Format(int32_t bufferSize, const char* format, ...);
const char*             String_FormatV(int32_t bufferSize, const char* format, va_list argv);

const char*             String_From(void* buffer, int32_t bufferSize, const char* source);
const char*             String_FormatBuffer(void* buffer, int32_t bufferSize, const char* format, ...);
const char*             string_FormatBufferV(void* buffer, int32_t bufferSize, const char* format, va_list argv);

int32_t                 String_GetLength(const char* target);
int32_t                 String_CalcLength(const char* target);

const StringBuffer*     String_GetBuffer(const char* target);

bool                    String_IsHeap(const char* target);
bool                    String_IsWeak(const char* target);
bool                    String_IsSmart(const char* target);
bool                    String_IsInvalidPointer(const char* target);

#ifdef __cplusplus
}
#endif

inline bool String_IsHeap(const char* target)
{
    assert(target != nullptr && "Attempt to use null-pointer on string");

    StringBuffer* buffer = (StringBuffer*)(target - sizeof(StringBuffer));
    return buffer->memtag == STRING_MEMTAG_HEAP;
}

inline bool String_IsWeak(const char* target)
{
    assert(target != nullptr && "Attempt to use null-pointer on string");

    StringBuffer* buffer = (StringBuffer*)(target - sizeof(StringBuffer));
    return buffer->memtag == STRING_MEMTAG_WEAK;
}

inline bool String_IsSmart(const char* target)
{
    assert(target != nullptr && "Attempt to use null-pointer on string");

    StringBuffer* buffer = (StringBuffer*)(target - sizeof(StringBuffer));
    return buffer->memtag == STRING_MEMTAG_HEAP || buffer->memtag == STRING_MEMTAG_WEAK;
}

inline bool String_IsInvalidPointer(const char* target)
{
    return (uintptr_t)target > sizeof(StringBuffer);
}

inline int32_t String_GetLength(const char* target)
{
    assert(target != nullptr && "Attempt to use null-pointer on string");

    if (String_IsSmart(target))
    {
        StringBuffer* buffer = (StringBuffer*)(target - sizeof(StringBuffer));
        return buffer->length;
    }
    else
    {
        return String_CalcLength(target);
    }
}

inline const StringBuffer* String_GetBuffer(const char* target)
{
    assert(target != nullptr && "Attempt to use null-pointer on string");
    assert(String_IsInvalidPointer(target) && "Attempt to use invalid-pointer on string");

    return (StringBuffer*)(target - sizeof(StringBuffer));
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
