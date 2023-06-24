#pragma once

typedef enum LogLevel
{
    LogLevel_Info,
    LogLevel_Warn,
    LogLevel_Error,
} LogLevel;

typedef struct Logger
{
    Logger* next;
    void*   data;

    void (*LogFn)(void* data, LogLevel level, const char* tag, const char* text);
} Logger;

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

void    Log_InfoImpl(const char* tag, const char* fmt, ...);
void    Log_WarnImpl(const char* tag, const char* fmt, ...);
void    Log_ErrorImpl(const char* tag, const char* fmt, ...);

void    Log_AddLogger(Logger* logger);
void    Log_SetLogger(Logger* logger);

Logger* Log_GetTTYLogger(void);
Logger* Log_GetRootLogger(void);
Logger* Log_GetLastLogger(void);

#ifdef __cplusplus
}
#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++

