#pragma once

typedef enum LogLevel
{
    LogLevel_Info,
    LogLevel_Warn,
    LogLevel_Error,
} LogLevel;

typedef struct Logger Logger;
struct Logger
{
    Logger* next;
    void*   data;

    void (*LogFn)(void* data, LogLevel level, const char* tag, const char* text);
};

typedef struct LogRecord LogRecord;
struct LogRecord
{
    LogRecord*      next;

    LogLevel        level;
    const char*     tag;
    const char*     text;
};

typedef struct LogStorage LogStorage;
struct LogStorage
{
    int             count;
    LogRecord*      head;
    LogRecord*      tail;
    LogRecord*      free;

    LogStorage*     prev;
    LogStorage*     current;
};

#ifdef LOGGING
#define Log_Info(tag, fmt, ...)  Log_InfoImpl(tag, fmt, ##__VA_ARGS__)
#define Log_Warn(tag, fmt, ...)  Log_WarnImpl(tag, fmt, ##__VA_ARGS__)
#define Log_Error(tag, fmt, ...) Log_ErrorImpl(tag, fmt, ##__VA_ARGS__)
#else
#define Log_Info(tag, fmt, ...)  ((void)0)
#define Log_Warn(tag, fmt, ...)  ((void)0)
#define Log_Error(tag, fmt, ...) ((void)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

void        Log_InfoImpl(const char* tag, const char* fmt, ...);
void        Log_WarnImpl(const char* tag, const char* fmt, ...);
void        Log_ErrorImpl(const char* tag, const char* fmt, ...);

void        Log_AddLogger(Logger* logger);
void        Log_SetLogger(Logger* logger);

Logger*     Log_GetTTYLogger(void);
Logger*     Log_GetRootLogger(void);
Logger*     Log_GetLastLogger(void);

LogStorage* LogStorage_Create(int recordCount);
void        LogStorage_Destroy(LogStorage* storage);
void        LogStorage_AddRecord(LogStorage* storage, LogLevel level, const char* tag, const char* text);
void        LogStorage_FreeRecord(LogStorage* storage, LogRecord* record);
Logger      LogStorage_GetLogger(LogStorage* storage);

#ifdef __cplusplus
}
#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++

