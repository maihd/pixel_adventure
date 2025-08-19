#pragma once

#include <stdint.h>

typedef int32_t     (ThreadFunc)(void*);
typedef void        (ThreadFiberFunc)(void*);

// @todo: convert to C ABI
struct ThreadMutex
{
    void*       handle = nullptr;

    void        Create();
    void        Destroy();

    void        Lock();
    void        Unlock();
    bool        TryLock();
};

// @todo: convert to C ABI
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

// @todo: convert to C ABI
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

// @todo: convert to C ABI
struct ThreadFiber
{
    void*               handle;

    ThreadFiberFunc*    func;
    void*               data;

    void                Create();
    void                Destroy();
    void                SwitchTo();
};

// @todo: convert to C ABI
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

// @todo: convert to C ABI
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
