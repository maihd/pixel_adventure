#include <assert.h>
#include <SDL2/SDL.h>

#include "Native/Thread.h"

static uint64_t gMainThreadId;

void ThreadSystem::Setup()
{
    gMainThreadId = (uint64_t)SDL_GetThreadID(nullptr);
}

void ThreadSystem::Shutdown()
{
    assert(gMainThreadId == SDL_GetThreadID(nullptr));

    gMainThreadId = 0;
}

bool ThreadSystem::IsMainThread()
{
    return gMainThreadId == (uint64_t)SDL_GetThreadID(nullptr);
}

uint32_t ThreadSystem::GetMainThreadId()
{
    return (uint32_t)gMainThreadId;
}

int32_t ThreadSystem::GetCpuCores()
{
    return SDL_GetCPUCount();
}

int64_t ThreadSystem::GetCpuTicks()
{
    return SDL_GetPerformanceCounter();
}

int64_t ThreadSystem::GetCpuFrequency()
{
    return SDL_GetPerformanceFrequency();
}

void ThreadSystem::Sleep(int64_t miliseconds)
{
    SDL_Delay(miliseconds);
}

void ThreadSystem::MicroSleep(int64_t microseconds)
{
    SDL_Delay(microseconds / 1000);

#if 0
    /* 'NTSTATUS NTAPI NtDelayExecution(BOOL Alerted, PLARGE_INTEGER time);' */
    /* 'typedef LONG NTSTATUS;' =)) */
    /* '#define NTAPI __stdcall' =)) */
    typedef LONG(__stdcall * NtDelayExecutionFN)(BOOL, PLARGE_INTEGER);
    
    static bool done_finding;
    static NtDelayExecutionFN NtDelayExecution;
    
    if (!NtDelayExecution && !done_finding)
    {
        done_finding = 1;
        HMODULE module = GetModuleHandleA("ntdll.dll");
        const char* func = "NtDelayExecution";
        NtDelayExecution = (NtDelayExecutionFN)GetProcAddress(module, func);
    }
    
    if (NtDelayExecution)
    {
        LARGE_INTEGER times;
        times.QuadPart = (LONGLONG)(-microseconds * 10); // Timer is precise as 100 nanoseconds
        NtDelayExecution(FALSE, &times);
    }
    else
    {
        Sleep((DWORD)(microseconds / (1000)));
    }
#endif
}

bool Thread::Start(ThreadFunc* func, void* data)
{
    SDL_Thread* handle = SDL_CreateThread(func, "", data);

    this->id        = (uint64_t)SDL_GetThreadID(handle);
    this->func      = func;
    this->data      = data;
    this->handle    = handle;

    return handle != NULL;
}

void Thread::Wait()
{
    int status;
    SDL_WaitThread((SDL_Thread*)handle, &status);
}

void ThreadMutex::Create()
{
    this->handle = SDL_CreateMutex();
}

void ThreadMutex::Destroy()
{
    SDL_DestroyMutex((SDL_mutex*)this->handle);
    this->handle = nullptr;
}

void ThreadMutex::Lock()
{
    SDL_LockMutex((SDL_mutex*)this->handle);
}

void ThreadMutex::Unlock()
{
    SDL_UnlockMutex((SDL_mutex*)this->handle);
}

bool ThreadMutex::TryLock()
{
    return SDL_TryLockMutex((SDL_mutex*)this->handle);
}

void ThreadSignal::Create()
{
    this->handle = SDL_CreateCond();
}

void ThreadSignal::Destroy()
{
    SDL_DestroyCond((SDL_cond*)this->handle);
    this->handle = nullptr;
}

void ThreadSignal::Wait(const ThreadMutex* mutex)
{
    this->Wait(*mutex);
}

void ThreadSignal::Wait(const ThreadMutex& mutex)
{
    SDL_CondWait((SDL_cond*)this->handle, (SDL_mutex*)mutex.handle);
}

void ThreadSignal::Signal()
{
    SDL_CondSignal((SDL_cond*)this->handle);
}

void ThreadSignal::Broadcast()
{
    SDL_CondBroadcast((SDL_cond*)this->handle);
}

void ThreadSemaphore::Create(int32_t value)
{
    this->value = value;
    this->handle = SDL_CreateSemaphore(value);
}

void ThreadSemaphore::Destroy()
{
    SDL_DestroySemaphore((SDL_sem*)this->handle);

    this->value = 0;
    this->handle = nullptr;
}

void ThreadSemaphore::Post()
{
    SDL_SemPost((SDL_sem*)this->handle);
    this->value = SDL_SemValue((SDL_sem*)this->handle);
}

void ThreadSemaphore::Wait()
{
    SDL_SemWait((SDL_sem*)this->handle);
    this->value = SDL_SemValue((SDL_sem*)this->handle);
}

bool ThreadSemaphore::TryWait()
{
    const bool result = SDL_SemTryWait((SDL_sem*)this->handle);
    this->value = SDL_SemValue((SDL_sem*)this->handle);
    return result;
}

void ThreadFiber::Create()
{
    //fiber->handle = (uintptr_t)CreateFiber(0, (LPFIBER_START_ROUTINE)fiber->func, fiber->data);
}

void ThreadFiber::Destroy()
{
    //DeleteFiber((void*)fiber->handle);
    //fiber->handle = 0;
}

void ThreadFiber::SwitchTo()
{
    //SwitchToFiber((void*)fiber->handle);
}
