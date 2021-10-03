#pragma once

#include <stdint.h>

struct FileInterface;

namespace FileModes
{
    enum Type : uint32_t
    {
        None               = 0,
        Read               = 1 << 0,
        Write              = 1 << 1,
        Append             = 1 << 2,
        Binary             = 1 << 3,

        ReadBinary         = Read | Binary,
        WriteBinary        = Write | Binary,

        ReadWrite          = Read | Write,
        ReadWriteBinary    = Read | Write | Binary,
    };
}
using FileMode = FileModes::Type;

enum struct SeekOffset : uint32_t
{
    StartOfFile,
    CurrentPosition,
    EndOfFile,
};

struct MemoryStream
{
    enum Flags : uint32_t
    {
        BufferOwner = 1 << 0
    };

    void*               buffer;
    int32_t             length;
    int32_t             cursor;
    Flags               flags;
};

struct FileStream
{
    FileInterface*      inteface;
    union
    {
        void*           handle;
        MemoryStream    memory;
    };

    bool                Open(const char* path, const FileMode mode);
    bool                Open(void* buffer, int32_t bufferSize);
    bool                Close(void);

    int32_t             Read(void* outputBuffer, int32_t bufferSizeInBytes);
    int32_t             Write(const void* inputBuffer, int32_t bufferSizeInBytes);

    bool                Seek(SeekOffset offset, int32_t position);
    bool                Flush(void);

    int32_t             GetSize(void) const;
    int32_t             GetCursor(void) const;
    bool                IsAtEnd(void) const;
};

struct FileInterface
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
};

// -------------------------------------------------------------
// FileSystem functions
// -------------------------------------------------------------

namespace FileSystem
{
    bool            AddSearchPath(const char* path);
    bool            RemoveSearchPath(const char* path);

    bool            GetExistsPath(char* buffer, int32_t length, const char* path);

    //bool            GetAbsolutePath(char* buffer, int32_t length, const char* path);
    //bool            GetRelativePath(char* buffer, int32_t length, const char* path);

    bool            LoadZipFile(const char* path);
    bool            UnloadZipFile(const char* path);
}

// -------------------------------------------------------------
// FilePath functions
// -------------------------------------------------------------

// -------------------------------------------------------------
// FileStream functions
// -------------------------------------------------------------

inline bool FileStream::Close(void)
{
    return this->inteface->Close(this);
}

inline int32_t FileStream::Read(void* outputBuffer, int32_t bufferSizeInBytes)
{
    return this->inteface->Read(this, outputBuffer, bufferSizeInBytes);
}

inline int32_t FileStream::Write(const void* inputBuffer, int32_t bufferSizeInBytes)
{
    return this->inteface->Write(this, inputBuffer, bufferSizeInBytes);
}

inline bool FileStream::Seek(SeekOffset offset, int32_t position)
{
    return this->inteface->Seek(this, offset, position);
}

inline int32_t FileStream::GetSize(void) const
{
    return this->inteface->GetSize(this);
}

inline int32_t FileStream::GetCursor(void) const
{
    return this->inteface->GetCursor(this);
}

inline bool FileStream::Flush(void)
{
    return this->inteface->Flush(this);
}

inline bool FileStream::IsAtEnd(void) const
{
    return this->inteface->IsAtEnd(this);
}
