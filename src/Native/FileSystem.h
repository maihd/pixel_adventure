#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "Misc/Compiler.h"

struct FileInterface;

typedef uint32_t FileMode;
enum FileModes __enum_type(uint32_t)
{
    FileMode_None               = 0,
    FileMode_Read               = 1 << 0,
    FileMode_Write              = 1 << 1,
    FileMode_Append             = 1 << 2,
    FileMode_Binary             = 1 << 3,

    FileMode_ReadBinary         = FileMode_Read | FileMode_Binary,
    FileMode_WriteBinary        = FileMode_Write | FileMode_Binary,

    FileMode_ReadWrite          = FileMode_Read | FileMode_Write,
    FileMode_ReadWriteBinary    = FileMode_Read | FileMode_Write | FileMode_Binary,
};

typedef enum SeekOffset __enum_type(uint32_t)
{
    SeekOffset_StartOfFile,
    SeekOffset_CurrentPosition,
    SeekOffset_EndOfFile,
} SeekOffset;

// @todo: convert to C ABI
typedef struct MemoryStream
{
    void*               buffer;
    int32_t             length;
    int32_t             cursor;
    uint32_t            flags;
} MemoryStream;

enum MemoryStreamFlags __enum_type(uint32_t)
{
    MemoryStreamFlags_None          = 0,
    MemoryStreamFlags_BufferOwner   = 1 << 0
};

// @todo: convert to C ABI
typedef struct FileStream
{
    FileInterface*      inteface;
    union
    {
        void*           handle;
        MemoryStream    memory;
    };
} FileStream;

typedef struct FileInterface
{
    void*                 const Open;
    bool                (*const Close)(FileStream* stream);

    int32_t             (*const Read)(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes);
    int32_t             (*const Write)(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes);

    bool                (*const Seek)(FileStream* stream, SeekOffset offset, int32_t position);
    int32_t             (*const GetSize)(const FileStream* stream);
    int32_t             (*const GetCursor)(const FileStream* stream);
    
    bool                (*const Flush)(FileStream* stream);
    bool                (*const IsAtEnd)(const FileStream* stream);
} FileInterface;

// -------------------------------------------------------------
// FileSystem functions
// -------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

bool            FileSystem_AddSearchPath(const char* path);
bool            FileSystem_RemoveSearchPath(const char* path);

bool            FileSystem_GetExistsPath(char* buffer, int32_t length, const char* path);

//bool          FileSystem_GetAbsolutePath(char* buffer, int32_t length, const char* path);
//bool          FileSystem_GetRelativePath(char* buffer, int32_t length, const char* path);

bool            FileSystem_LoadZipFile(const char* path);
bool            FileSystem_UnloadZipFile(const char* path);

bool            FileStream_Open(FileStream* stream, const char* path, FileMode mode);
bool            FileStream_FromMemory(FileStream* stream, void* buffer, int32_t bufferSize);

#ifdef __cplusplus
}
#endif

// -------------------------------------------------------------
// FilePath functions
// -------------------------------------------------------------

// -------------------------------------------------------------
// FileStream functions
// -------------------------------------------------------------

inline bool FileStream_Close(FileStream* stream)
{
    return stream->inteface->Close(stream);
}

inline int32_t FileStream_Read(FileStream* stream, void* outputBuffer, int32_t bufferSizeInBytes)
{
    return stream->inteface->Read(stream, outputBuffer, bufferSizeInBytes);
}

inline int32_t FileStream_Write(FileStream* stream, const void* inputBuffer, int32_t bufferSizeInBytes)
{
    return stream->inteface->Write(stream, inputBuffer, bufferSizeInBytes);
}

inline bool FileStream_Seek(FileStream* stream, SeekOffset offset, int32_t position)
{
    return stream->inteface->Seek(stream, offset, position);
}

inline int32_t FileStream_GetSize(const FileStream* stream)
{
    return stream->inteface->GetSize(stream);
}

inline int32_t FileStream_GetCursor(const FileStream* stream)
{
    return stream->inteface->GetCursor(stream);
}

inline bool FileStream_Flush(FileStream* stream)
{
    return stream->inteface->Flush(stream);
}

inline bool FileStream_IsAtEnd(const FileStream* stream)
{
    return stream->inteface->IsAtEnd(stream);
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
