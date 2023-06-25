#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdarg.h>
#include "Text/String.h"
#include "Misc/Logging.h"
#include "Native/Memory.h"

const char* LOG_LEVEL_HEADER[] = {
    "Info",
    "Warn",
    "Error",
};

void DefaultLogger_Log(void* data, LogLevel level, const char* tag, const char* text)
{
    printf("[%s][%s] %s\n", LOG_LEVEL_HEADER[(int)level], tag, text);
}

static Logger s_defaultLogger = {
    /* .next   = */ NULL,
    /* .data   = */ NULL,
    /* .LogFn  = */ DefaultLogger_Log,
};

static Logger* s_rootLogger = &s_defaultLogger;
static Logger* s_lastLogger = &s_defaultLogger;

static char s_logBuffer[4096];

void LogAllLoggers(LogLevel level, const char* tag, const char* text)
{
    for (Logger* logger = s_rootLogger; logger != NULL; logger = logger->next)
    {
        logger->LogFn(logger->data, level, tag, text);
    }
}

void Log_InfoImpl(const char* tag, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    
    vsprintf(s_logBuffer, fmt, arg);
    
    LogAllLoggers(LogLevel_Info, tag, s_logBuffer);

    va_end(arg);
}

void Log_WarnImpl(const char* tag, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    
    vsprintf(s_logBuffer, fmt, arg);
    
    LogAllLoggers(LogLevel_Warn, tag, s_logBuffer);

    va_end(arg);
}

void Log_ErrorImpl(const char* tag, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    
    vsprintf(s_logBuffer, fmt, arg);
    
    LogAllLoggers(LogLevel_Error, tag, s_logBuffer);

    va_end(arg);
}

void Log_AddLogger(Logger* logger)
{
    s_lastLogger->next = logger;
    s_lastLogger = logger;
}

void Log_SetLogger(Logger* logger)
{
    s_rootLogger = logger;
    s_lastLogger = logger;

    s_defaultLogger.next = NULL;
}

Logger* Log_GetTTYLogger(void)
{
    return &s_defaultLogger;
}

Logger* Log_GetRootLogger(void)
{
    return s_rootLogger;
}

Logger* Log_GetLastLogger(void)
{
    return s_rootLogger;
}

LogStorage* LogStorage_Create(int recordCount)
{
    const int HEADER_REVERSE = sizeof(LogStorage);
    const int allocSize = HEADER_REVERSE + recordCount * sizeof(LogRecord);
    void* block = MemoryAllocTag("LogStorage", allocSize, alignof(LogRecord));
    
    LogStorage* storage = (LogStorage*)block;

    LogRecord* first = (LogRecord*)((char*)block + HEADER_REVERSE);
    LogRecord* record = first;
    for (int i = 0; i < recordCount; i++)
    {
        record = record->next = record + 1;
    }
    record->next = NULL;

    storage->count = recordCount;
    storage->head = NULL;
    storage->tail = NULL;
    storage->free = first;

    storage->prev = NULL;
    storage->current = storage;

    return storage;
}

void LogStorage_Destroy(LogStorage* storage)
{
    storage = storage->current;
    while (storage != NULL)
    {
        for (LogRecord* record = storage->head; record != nullptr; record = record->next)
        {
            String_Free(record->text);
        }

        LogStorage* prev = storage->prev;
        MemoryFreeTag("LogStorage", storage);
        storage = prev;
    }
}

void LogStorage_AddRecord(LogStorage* storage, LogLevel level, const char* tag, const char* text)
{
    LogStorage* current = storage->current;
    if (current->free == NULL)
    {
        storage->current = LogStorage_Create(storage->count);
        current->current = storage->current;
        current = current->current;
    }

    LogRecord* result = storage->free;
    if (current->tail)
    {
        current->tail->next = result;
    }
    else
    {
        current->head = result;
    }

    current->free   = current->free->next;
    current->tail   = result;
    result->next    = NULL;

    result->level   = level;
    result->tag     = String_Ref(tag);
    result->text    = String_New(text);

    //return result;
}

void LogStorage_FreeRecord(LogStorage* storage, LogRecord* record)
{
    LogStorage* current = storage->current;
    record->next = current->free;
    current->free = record;
}

static void LogStorage_LogFn(void* data, LogLevel level, const char* tag, const char* text)
{
    LogStorage_AddRecord((LogStorage*)data, level, tag, text);
}

Logger LogStorage_GetLogger(LogStorage* storage)
{
    return {
        NULL,
        storage,
        LogStorage_LogFn
    };
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
