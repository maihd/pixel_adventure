#pragma once

#include <stdint.h>

typedef int32_t     (ThreadFunc)(void*);
typedef void        (ThreadFiberFunc)(void*);

struct ThreadMutex
{
    void*       handle = nullptr;

    void        Create();
    void        Destroy();

    void        Lock();
    void        Unlock();
    bool        TryLock();
};

struct ThreadSignal
{
    void*       handle = nullptr;

    void        Create(void);
    void        Destroy(void);

    void        Wait(const ThreadMutex* mutex);
    void        Wait(const ThreadMutex& mutex);

    void        Signal(void);
    void        Broadcast(void);
};

struct ThreadSemaphore
{
    void*       handle  = nullptr;
    int32_t     value   = 0;

    void        Create(int32_t value);
    void        Destroy(void);

    void        Post(void);
    void        Wait(void);
    bool        TryWait(void);
};

struct ThreadFiber
{
    void*               handle;

    ThreadFiberFunc*    func;
    void*               data;

    void                Create();
    void                Destroy();
    void                SwitchTo();
};

struct Thread
{
    uint64_t            id;
    void*               handle;
    void*               fiberHandle;

    //void*           stack;
    //const char*     name;
    //int32_t         preferedCore;
    //int32_t         migrateEnabled;

    ThreadFunc*         func;
    void*               data;

    bool                Start(ThreadFunc* func, void* data);
    void                Wait(void);
};

namespace ThreadSystem
{
    void        Setup();
    void        Shutdown();

    bool        IsMainThread();
    uint32_t    GetMainThreadId();

    int32_t     GetCpuCores();
    int64_t     GetCpuTicks();
    int64_t     GetCpuFrequency();

    void        Sleep(int64_t miliseconds);
    void        MicroSleep(int64_t microseconds);
}
