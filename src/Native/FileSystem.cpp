#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Memory.h"
#include "FileSystem.h"

enum
{
    SEARCH_PATH_MAX_COUNT   = 16,
    SEARCH_PATH_MAX_LENGTH  = 1024,
};

static int32_t  gSearchPathCount;
static char     gSearchPaths[SEARCH_PATH_MAX_COUNT][SEARCH_PATH_MAX_LENGTH];

static int32_t IndexOfSearchPath(const char* path)
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

bool FileSystem::AddSearchPath(const char* path)
{
    if (IndexOfSearchPath(path) == -1)
    {
        assert(gSearchPathCount < SEARCH_PATH_MAX_COUNT);
        strcpy(gSearchPaths[gSearchPathCount++], path);
        return true;
    }

    return false;
}

bool FileSystem::RemoveSearchPath(const char* path)
{
    int32_t index = IndexOfSearchPath(path);
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

bool FileSystem::GetExistsPath(char* buffer, int32_t length, const char* path)
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

//bool FileSystem::GetAbsolutePath(char* buffer, int32_t length, const char* path);
//bool FileSystem::GetRelativePath(char* buffer, int32_t length, const char* path);

// -------------------------------------------------------------
// FileInterface functions prototype
// -------------------------------------------------------------

namespace FileSystem
{
    namespace MemoryStream
    {
        static bool     Open(FileInterface* inteface, void* buffer, int32_t bufferSizeInBytes, FileStream* outStream);
        static bool     Close(FileStream* inteface);

        static int32_t  Read(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes);
        static int32_t  Write(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes);

        static bool     Seek(FileStream* stream, SeekOffset offset, int32_t count);
        static int32_t  GetSize(const FileStream* stream);
        static int32_t  GetCursor(const FileStream* stream);

        static bool     Flush(FileStream* stream);
        static bool     IsAtEnd(const FileStream* stream);
    }

    namespace StdIO
    {
        static bool     Open(FileInterface* system, const char* fileNamee, FileMode mode, FileStream* outStream);
        static bool     Close(FileStream* stream);

        static int32_t  Read(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes);
        static int32_t  Write(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes);

        static bool     Seek(FileStream* stream, SeekOffset offset, int32_t count);
        static int32_t  GetSize(const FileStream* stream);
        static int32_t  GetCursor(const FileStream* stream);

        static bool     Flush(FileStream* stream);
        static bool     IsAtEnd(const FileStream* stream);
    }
}

// -------------------------------------------------------------
// FileInterfaces
// -------------------------------------------------------------

static FileInterface gMemoryFileSystem = {
    FileSystem::MemoryStream::Open,
    FileSystem::MemoryStream::Close,

    FileSystem::MemoryStream::Read,
    FileSystem::MemoryStream::Write,

    FileSystem::MemoryStream::Seek,
    FileSystem::MemoryStream::GetSize,
    FileSystem::MemoryStream::GetCursor,

    FileSystem::MemoryStream::Flush,
    FileSystem::MemoryStream::IsAtEnd
};

static FileInterface gStdFileSystem = {
    FileSystem::StdIO::Open,
    FileSystem::StdIO::Close,

    FileSystem::StdIO::Read,
    FileSystem::StdIO::Write,

    FileSystem::StdIO::Seek,
    FileSystem::StdIO::GetSize,
    FileSystem::StdIO::GetCursor,

    FileSystem::StdIO::Flush,
    FileSystem::StdIO::IsAtEnd
};

// -------------------------------------------------------------
// FileStream functions
// -------------------------------------------------------------

bool FileStream::Open(const char* path, FileMode mode)
{
    return FileSystem::StdIO::Open(&gStdFileSystem, path, mode, this);
}

bool FileStream::Open(void* buffer, int32_t bufferSize)
{
    return FileSystem::MemoryStream::Open(&gMemoryFileSystem, buffer, bufferSize, this);
}

// -------------------------------------------------------------
// FileInterface functions define
// -------------------------------------------------------------

bool FileSystem::MemoryStream::Open(FileInterface* inteface, void* inputBuffer, int32_t bufferSizeInBytes, FileStream* outStream)
{
    FileStream stream;
    stream.inteface      = inteface;
    stream.memory.buffer = inputBuffer;
    stream.memory.length = bufferSizeInBytes;
    stream.memory.cursor = 0;
    stream.memory.flags  = (::MemoryStream::Flags)0;

    *outStream = stream;
    return true;
}

bool FileSystem::MemoryStream::Close(FileStream* stream)
{
    if (stream && stream->inteface == &gMemoryFileSystem)
    {
        if (stream->memory.flags & ::MemoryStream::BufferOwner)
        {
            Memory_Free(stream->memory.buffer);
        }

        return true;
    }

    return false;
}

int32_t FileSystem::MemoryStream::Read(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes)
{
    const int32_t readBytes = stream->memory.cursor + bufferSizeInBytes > stream->memory.length 
        ? stream->memory.length - stream->memory.cursor
        : bufferSizeInBytes;

    memcpy(outputBuffer, (uint8_t*)stream->memory.buffer + stream->memory.cursor, readBytes);
    stream->memory.cursor += readBytes;
    return readBytes;
}

int32_t FileSystem::MemoryStream::Write(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes)
{
    const int32_t writeBytes = stream->memory.cursor + bufferSizeInBytes > stream->memory.length
        ? stream->memory.length - stream->memory.cursor
        : bufferSizeInBytes;

    memcpy((uint8_t*)stream->memory.buffer + stream->memory.cursor, inputBuffer, writeBytes);
    stream->memory.cursor += writeBytes;
    return writeBytes;
}

bool FileSystem::MemoryStream::Seek(FileStream* stream, SeekOffset offset, int32_t count)
{
    int32_t cursor;
    switch (offset)
    {
    case SeekOffset::CurrentPosition:
        cursor = stream->memory.cursor + count;
        break;

    case SeekOffset::EndOfFile:
        cursor = stream->memory.length - count;
        break;

    case SeekOffset::StartOfFile:
        cursor = count;
        break;

    default:
        assert(false && "Invalid seek offset");
        cursor = -1;
        break;
    }

    if (cursor < 0 && cursor >= stream->memory.length)
    {
        return false;
    }

    stream->memory.cursor = cursor;
    return true;
}

int32_t FileSystem::MemoryStream::GetSize(const FileStream* stream)
{
    return stream->memory.length;
}

int32_t FileSystem::MemoryStream::GetCursor(const FileStream* stream)
{
    return stream->memory.cursor;
}

bool FileSystem::MemoryStream::Flush(FileStream* stream)
{
    return true;
}

bool FileSystem::MemoryStream::IsAtEnd(const FileStream* stream)
{
    return stream->memory.cursor == stream->memory.length;
}

bool FileSystem::StdIO::Open(FileInterface* system, const char* fileName, FileMode mode, FileStream* outStream)
{
    char stdMode[6];
    char* ptrMode = stdMode;

    if (mode & FileMode::Read)
    {
        *ptrMode++ = 'r';
    }

    if (mode & FileMode::Write)
    {
        *ptrMode++ = 'w';
    }

    if (mode & FileMode::Binary)
    {
        *ptrMode++ = 'b';
    }

    if (mode & FileMode::Append)
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
    stream.inteface = system;
    stream.handle   = handle;

    *outStream = stream;
    return true;
}

bool FileSystem::StdIO::Close(FileStream* stream)
{
    if (stream && stream->inteface == &gStdFileSystem)
    {
        fclose((FILE*)stream->handle);
        stream->handle = NULL;
        return true;
    }

    return false;
}

int32_t FileSystem::StdIO::Read(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes)
{
    return (int32_t)fread(outputBuffer, 1, bufferSizeInBytes, (FILE*)stream->handle);
}

int32_t FileSystem::StdIO::Write(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes)
{
    return (int32_t)fwrite(inputBuffer, 1, bufferSizeInBytes, (FILE*)stream->handle);
}

bool FileSystem::StdIO::Seek(FileStream* stream, SeekOffset offset, int32_t count)
{
    long whence;
    switch (offset)
    {
    case SeekOffset::CurrentPosition:
        whence = SEEK_CUR;
        break;

    case SeekOffset::EndOfFile:
        whence = SEEK_END;
        break;

    case SeekOffset::StartOfFile:
        whence = SEEK_SET;
        break;

    default:
        assert(false && "Invalid seek offset");
        whence = -1;
        break;
    }

    return fseek((FILE*)stream->handle, whence, count) > 0;
}

int32_t FileSystem::StdIO::GetSize(const FileStream* stream)
{
    int32_t cursor = ftell((FILE*)stream->handle);

    fseek((FILE*)stream->handle, SEEK_END, 0);
    int32_t size = ftell((FILE*)stream->handle);
    fseek((FILE*)stream->handle, SEEK_SET, cursor);

    return size;
}

int32_t FileSystem::StdIO::GetCursor(const FileStream* stream)
{
    return ftell((FILE*)stream->handle);
}

bool FileSystem::StdIO::Flush(FileStream* stream)
{
    return fflush((FILE*)stream->handle) != 0;
}

bool FileSystem::StdIO::IsAtEnd(const FileStream* stream)
{
    return feof((FILE*)stream->handle);
}
