#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdarg.h>
#include "Misc/Logging.h"

void DefaultLogger_Log(void* data, LogLevel level, const char* tag, const char* text)
{
    printf("[%d][%s] %s", (int)level, tag, text);
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
    
    sprintf(s_logBuffer, fmt, arg);
    
    LogAllLoggers(LogLevel_Info, tag, s_logBuffer);

    va_end(arg);
}

void Log_WarnImpl(const char* tag, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    
    sprintf(s_logBuffer, fmt, arg);
    
    LogAllLoggers(LogLevel_Warn, tag, s_logBuffer);

    va_end(arg);
}

void Log_ErrorImpl(const char* tag, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    
    sprintf(s_logBuffer, fmt, arg);
    
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

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
