#pragma once

#include <stdint.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

typedef void (JobFunc)(void* items);

#ifdef __cplusplus
extern "C" {
#endif

void                JobSystem_Setup(void);
void                JobSystem_Shutdown(void);

bool                JobSystem_IsIdle(void);
void                JobSystem_WaitIdle(void);

void                JobSystem_QueueJob(JobFunc* func, void* items);

#ifdef __cplusplus
}
#endif
