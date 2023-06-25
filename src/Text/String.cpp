#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "Text/String.h"
#include "Native/Memory.h"
#include "Native/AtomicOps.h"

constexpr char* EMPTY_STRING = "";
constexpr int32_t STRING_ALIGNMENT = 4;

StringBuffer* StringBuffer_New(int32_t length)
{
    StringBuffer* buffer = (StringBuffer*)MemoryAllocTag("String", length + 1 + sizeof(StringBuffer), STRING_ALIGNMENT);
    assert(buffer != NULL && "Cannot create new buffer, maybe the system have been out of memory.");

    buffer->memref      = 1;
    buffer->memtag      = STRING_MEMTAG_HEAP;
    buffer->length      = length;
    buffer->capacity    = length + 1;
    return buffer;
}

const char* String_Ref(const char* source)
{
    assert(source != nullptr && "Attempt to use null-pointer on string");

    if (String_IsHeap(source))
    {
        StringBuffer* buffer = (StringBuffer*)(source - sizeof(StringBuffer));
        Atomic_AddI32(&buffer->memref, 1);
        return source;
    }

    return source;
}

const char* String_New(const char* source)
{
    assert(source != nullptr && "Attempt to use null-pointer on string");

    if (String_IsHeap(source))
    {
        StringBuffer* buffer = (StringBuffer*)(source - sizeof(StringBuffer));
        Atomic_AddI32(&buffer->memref, 1);
        return source;
    }

    const int32_t length = String_GetLength(source);
    if (length == 0)
    {
        return EMPTY_STRING;
    }
    else
    {
        StringBuffer* buffer = StringBuffer_New(length);
        memcpy(buffer->data, source, length + 1);
        return buffer->data;
    }
}

void String_Free(const char* target)
{
    assert(target != nullptr && "Attempt to use null-pointer on string");

    if (String_IsHeap(target))
    {
        StringBuffer* buffer = (StringBuffer*)(target - sizeof(StringBuffer));
        
        const int32_t memref = Atomic_SubI32(&buffer->memref, 1);
        if (memref <= 0)
        {
            MemoryFreeTag("String", buffer);
        }
    }   
}

const char* String_From(void* buffer, int32_t bufferSize, const char* source)
{
    assert(buffer != nullptr && "Attempt to use null-pointer on buffer");
    assert(source != nullptr && "Attempt to use null-pointer on string");

    const int32_t length = String_GetLength(source);
    if (length == 0)
    {
        return EMPTY_STRING;
    }
    else
    {
        assert(bufferSize >= length && "Buffer is too small");
        StringBuffer* stringBuffer = (StringBuffer*)buffer;
        stringBuffer->length = length;
        stringBuffer->memtag = STRING_MEMTAG_WEAK;

        strncpy(stringBuffer->data, source, length);
        stringBuffer->data[length] = '\0';
        
        return stringBuffer->data;
    }
}

const char* String_Format(int32_t bufferSize, const char* format, ...)
{
    assert(format != nullptr && "Attempt to use null-pointer on string");

    StringBuffer* buffer = StringBuffer_New(bufferSize);

    va_list argv;
    va_start(argv, format);
    int32_t length = (int32_t)vsnprintf(buffer->data, bufferSize, format, argv);
    va_end(argv);

    buffer->length = length;
    return buffer->data;
}

const char* String_FormatV(int32_t bufferSize, const char* format, va_list argv)
{
    assert(format != nullptr && "Attempt to use null-pointer on string");

    StringBuffer* buffer = StringBuffer_New(bufferSize + sizeof(StringBuffer));
    buffer->length = (int32_t)vsnprintf(buffer->data, bufferSize, format, argv);
    return buffer->data;
}

const char* String_FormatBuffer(void* buffer, int32_t bufferSize, const char* format, ...)
{
    assert(buffer != nullptr && "Attempt to use null-pointer on buffer");
    assert(format != nullptr && "Attempt to use null-pointer on string");

    StringBuffer* stringBuffer = (StringBuffer*)(buffer);

    va_list argv;
    va_start(argv, format);
    int32_t length = (int32_t)vsnprintf(stringBuffer->data, bufferSize - sizeof(StringBuffer), format, argv);
    va_end(argv);

    stringBuffer->length = length;
    stringBuffer->memtag = STRING_MEMTAG_WEAK;
    stringBuffer->capacity = bufferSize - sizeof(StringBuffer);
    
    return stringBuffer->data;
}

const char* String_FormatBufferV(void* buffer, int32_t bufferSize, const char* format, va_list argv)
{
    assert(buffer != nullptr && "Attempt to use null-pointer on buffer");
    assert(format != nullptr && "Attempt to use null-pointer on string");

    StringBuffer* stringBuffer = (StringBuffer*)(buffer);

    stringBuffer->length = (int32_t)vsnprintf(stringBuffer->data, bufferSize - sizeof(StringBuffer), format, argv);
    stringBuffer->memtag = STRING_MEMTAG_WEAK;
    stringBuffer->capacity = bufferSize - sizeof(StringBuffer);
    
    return stringBuffer->data;
}

int32_t String_CalcLength(const char* target)
{
    assert(target != nullptr && "Attempt to use null-pointer on target");
    return (int32_t)strlen(target);
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
