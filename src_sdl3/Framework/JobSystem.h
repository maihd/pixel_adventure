#pragma once

#include <stdint.h>

typedef void (JobFunc)(void* items);

namespace JobSystem
{
    void    Setup(void);
    void    Shutdown(void);

    bool    IsIdle(void);
    void    WaitIdle(void);

    void    Queue(JobFunc* func, void* items);
}
