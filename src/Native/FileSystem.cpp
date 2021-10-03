#include "FileSystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum
{
    SEARCH_PATH_MAX_COUNT   = 16,
    SEARCH_PATH_MAX_LENGTH  = 1024,
};

static int32_t  gSearchPathCount;
static char     gSearchPaths[SEARCH_PATH_MAX_COUNT][SEARCH_PATH_MAX_LENGTH];

static int32_t FileSystem_IndexOfSearchPath(const char* path)
{
    for (int32_t i = 0; i < gSearchPathCount; i++)
    {
        if (strcmp(gSearchPaths[i], path) == 0)
        {
            return i;
        }
    }

    return -1;
}

bool FileSystem_AddSearchPath(const char* path)
{
    if (FileSystem_IndexOfSearchPath(path) == -1)
    {
        assert(gSearchPathCount < SEARCH_PATH_MAX_COUNT);
        strcpy(gSearchPaths[gSearchPathCount++], path);
        return true;
    }

    return false;
}

bool FileSystem_RemoveSearchPath(const char* path)
{
    int32_t index = FileSystem_IndexOfSearchPath(path);
    if (index > -1)
    {
        if (index < gSearchPathCount - 1)
        {
            memmove(&gSearchPaths[index], &gSearchPaths[index + 1], gSearchPathCount - index - 1);
        }

        gSearchPathCount--;
        return true;
    }

    return false;
}

bool FileSystem_GetExistsPath(char* buffer, int32_t length, const char* path)
{
    FILE* file = fopen(path, "r");
    if (file)
    {
        fclose(file);
        snprintf(buffer, length, "%s", path);
        return true;
    }

    char tempBuffer[SEARCH_PATH_MAX_LENGTH * 2];
    for (int32_t i = 0; i < gSearchPathCount; i++)
    {
        const char* searchPath = gSearchPaths[i];
        snprintf(tempBuffer, sizeof(tempBuffer), "%s/%s", searchPath, path);
        
        FILE* file = fopen(tempBuffer, "r");
        if (file)
        {
            fclose(file);
            snprintf(buffer, length, "%s", tempBuffer);
            return true;
        }
    }

    return false;
}

bool FileSystem_GetAbsolutePath(char* buffer, int32_t length, const char* path);
bool FileSystem_GetRelativePath(char* buffer, int32_t length, const char* path);

// -------------------------------------------------------------
// FileSystem functions prototype
// -------------------------------------------------------------

static bool     FileSystem_MemoryStream_Open(FileSystem* system, void* buffer, int32_t bufferSizeInBytes, FileStream* outStream);
static bool     FileSystem_MemoryStream_Close(FileStream* stream);

static int32_t  FileSystem_MemoryStream_Read(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes);
static int32_t  FileSystem_MemoryStream_Write(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes);

static bool     FileSystem_MemoryStream_Seek(FileStream* stream, SeekBaseOffset offset, int32_t count);
static int32_t  FileSystem_MemoryStream_GetSize(const FileStream* stream);
static int32_t  FileSystem_MemoryStream_GetCursor(const FileStream* stream);

static bool     FileSystem_MemoryStream_Flush(FileStream* stream);
static bool     FileSystem_MemoryStream_IsAtEnd(const FileStream* stream);

static bool     FileSystem_StdIO_Open(FileSystem* system, const char* fileNamee, FileMode mode, FileStream* outStream);
static bool     FileSystem_StdIO_Close(FileStream* stream);

static int32_t  FileSystem_StdIO_Read(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes);
static int32_t  FileSystem_StdIO_Write(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes);

static bool     FileSystem_StdIO_Seek(FileStream* stream, SeekBaseOffset offset, int32_t count);
static int32_t  FileSystem_StdIO_GetSize(const FileStream* stream);
static int32_t  FileSystem_StdIO_GetCursor(const FileStream* stream);

static bool     FileSystem_StdIO_Flush(FileStream* stream);
static bool     FileSystem_StdIO_IsAtEnd(const FileStream* stream);

// -------------------------------------------------------------
// FileSystems
// -------------------------------------------------------------

static FileSystem gMemoryFileSystem = {
    FileSystem_MemoryStream_Close,

    FileSystem_MemoryStream_Read,
    FileSystem_MemoryStream_Write,

    FileSystem_MemoryStream_Seek,
    FileSystem_MemoryStream_GetSize,
    FileSystem_MemoryStream_GetCursor,

    FileSystem_MemoryStream_Flush,
    FileSystem_MemoryStream_IsAtEnd
};

static FileSystem gStdFileSystem = {
    FileSystem_StdIO_Close,

    FileSystem_StdIO_Read,
    FileSystem_StdIO_Write,

    FileSystem_StdIO_Seek,
    FileSystem_StdIO_GetSize,
    FileSystem_StdIO_GetCursor,

    FileSystem_StdIO_Flush,
    FileSystem_StdIO_IsAtEnd
};

// -------------------------------------------------------------
// FileStream functions
// -------------------------------------------------------------

bool FileStream_OpenFromPath(const char* path, FileMode mode, FileStream* outStream)
{
    return FileSystem_StdIO_Open(&gStdFileSystem, path, mode, outStream);
}

bool FileStream_OpenFromMemory(void* buffer, int32_t bufferSizeInBytes, FileStream* outStream)
{
    return FileSystem_MemoryStream_Open(&gMemoryFileSystem, buffer, bufferSizeInBytes, outStream);
}

// -------------------------------------------------------------
// FileSystem functions define
// -------------------------------------------------------------

bool FileSystem_MemoryStream_Open(FileSystem* system, void* inputBuffer, int32_t bufferSizeInBytes, FileStream* outStream)
{
    FileStream stream;
    stream.system = system;
    stream.memory.buffer = inputBuffer;
    stream.memory.length = bufferSizeInBytes;
    stream.memory.cursor = 0;
    stream.memory.flags  = 0;

    *outStream = stream;
    return true;
}

bool FileSystem_MemoryStream_Close(FileStream* stream)
{
    if (stream && stream->system == &gMemoryFileSystem)
    {
        if (stream->memory.flags & MemoryStreamFlags_BufferOwner)
        {
            _aligned_free(stream->memory.buffer);
        }

        return true;
    }

    return false;
}

int32_t FileSystem_MemoryStream_Read(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes)
{
    const int32_t readBytes = stream->memory.cursor + bufferSizeInBytes > stream->memory.length 
        ? stream->memory.length - stream->memory.cursor
        : bufferSizeInBytes;

    memcpy(outputBuffer, (uint8_t*)stream->memory.buffer + stream->memory.cursor, readBytes);
    stream->memory.cursor += readBytes;
    return readBytes;
}

int32_t FileSystem_MemoryStream_Write(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes)
{
    const int32_t writeBytes = stream->memory.cursor + bufferSizeInBytes > stream->memory.length
        ? stream->memory.length - stream->memory.cursor
        : bufferSizeInBytes;

    memcpy((uint8_t*)stream->memory.buffer + stream->memory.cursor, inputBuffer, writeBytes);
    stream->memory.cursor += writeBytes;
    return writeBytes;
}

bool FileSystem_MemoryStream_Seek(FileStream* stream, SeekBaseOffset offset, int32_t count)
{
    int32_t cursor;
    switch (offset)
    {
    case SeekBaseOffset_CurrentPosition:
        cursor = stream->memory.cursor + count;
        break;

    case SeekBaseOffset_EndOfFile:
        cursor = stream->memory.length - count;
        break;

    case SeekBaseOffset_StartOfFile:
        cursor = count;
        break;
    }

    if (cursor < 0 && cursor >= stream->memory.length)
    {
        return false;
    }

    stream->memory.cursor = cursor;
    return true;
}

int32_t FileSystem_MemoryStream_GetSize(const FileStream* stream)
{
    return stream->memory.length;
}

int32_t FileSystem_MemoryStream_GetCursor(const FileStream* stream)
{
    return stream->memory.cursor;
}

bool FileSystem_MemoryStream_Flush(FileStream* stream)
{
    return true;
}

bool FileSystem_MemoryStream_IsAtEnd(const FileStream* stream)
{
    return stream->memory.cursor == stream->memory.length;
}

bool FileSystem_StdIO_Open(FileSystem* system, const char* fileName, FileMode mode, FileStream* outStream)
{
    char stdMode[6];
    char* ptrMode = stdMode;

    if (mode & FileMode_Read)
    {
        *ptrMode++ = 'r';
    }

    if (mode & FileMode_Write)
    {
        *ptrMode++ = 'w';
    }

    if (mode & FileMode_Binary)
    {
        *ptrMode++ = 'b';
    }

    if (mode & FileMode_Append)
    {
        *ptrMode++ = '+';
    }

    *ptrMode++ = '\0';

    FILE* handle = fopen(fileName, stdMode);
    if (!handle)
    {
        return false;
    }

    FileStream stream;
    stream.system = system;
    stream.handle = handle;

    *outStream = stream;
    return true;
}

bool FileSystem_StdIO_Close(FileStream* stream)
{
    if (stream && stream->system == &gStdFileSystem)
    {
        fclose((FILE*)stream->handle);
        stream->handle = NULL;
        return true;
    }

    return false;
}

int32_t FileSystem_StdIO_Read(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes)
{
    return (int32_t)fread(outputBuffer, 1, bufferSizeInBytes, (FILE*)stream->handle);
}

int32_t FileSystem_StdIO_Write(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes)
{
    return (int32_t)fwrite(inputBuffer, 1, bufferSizeInBytes, (FILE*)stream->handle);
}

bool FileSystem_StdIO_Seek(FileStream* stream, SeekBaseOffset offset, int32_t count)
{
    long whence;
    switch (offset)
    {
    case SeekBaseOffset_CurrentPosition:
        whence = SEEK_CUR;
        break;

    case SeekBaseOffset_EndOfFile:
        whence = SEEK_END;
        break;

    case SeekBaseOffset_StartOfFile:
        whence = SEEK_SET;
        break;
    }

    return fseek((FILE*)stream->handle, whence, count) > 0;
}

int32_t FileSystem_StdIO_GetSize(const FileStream* stream)
{
    int32_t cursor = ftell((FILE*)stream->handle);

    fseek((FILE*)stream->handle, SEEK_END, 0);
    int32_t size = ftell((FILE*)stream->handle);
    fseek((FILE*)stream->handle, SEEK_SET, cursor);

    return size;
}

int32_t FileSystem_StdIO_GetCursor(const FileStream* stream)
{
    return ftell((FILE*)stream->handle);
}

bool FileSystem_StdIO_Flush(FileStream* stream)
{
    return fflush((FILE*)stream->handle) != 0;
}

bool FileSystem_StdIO_IsAtEnd(const FileStream* stream)
{
    return feof((FILE*)stream->handle);
}
